#include "syntax.h"

syntax_t::iterator::iterator()
{
}

syntax_t::iterator::iterator(string type, graph::iterator node, segment::iterator source)
{
	this->curr.token = token_t(type, source);
	this->node = node;
}

syntax_t::iterator::iterator(string type, graph::link_iterator branch, segment::iterator source)
{
	this->curr.token = token_t(type, source);
	this->branch = branch;
	this->node = *branch;
}

syntax_t::iterator::~iterator()
{
}

bool syntax_t::iterator::next()
{
	token_t result = (*node)->parse(curr.token.end(), &curr.context);
	if (result)
	{
		curr.token.append(result);
		return true;
	}
	else
		return false;
}

bool syntax_t::iterator::next_branch()
{
	if (!closest.token || curr.token.end() > closest.token.end())
	{
		closest.token = curr.token;
		closest.context = curr.context;
	}

	branch++;
	if (branch)
	{
		node = *branch;
		curr.token.reset();
		curr.context.reset();
		return true;
	}
	else
		return false;
}

syntax_t::syntax_t()
{
}

syntax_t::syntax_t(string type)
{
	this->type = type;
}

syntax_t::~syntax_t()
{
}

/*
Each syntax parse function has a stack of choice points.
Each element in the stack contains an iterator along the possible
choices, an iterator along the syntax graph, and a token.


*/
token_t syntax_t::parse(segment::iterator source, context_t *context)
{
	array<syntax_t::iterator> stack;
	stack.push_back(syntax_t::iterator(type, start.begin(), source));

	while (stack.size() > 0)
	{
		array<syntax_t::iterator>::iterator curr = stack.rbegin();

		token_t skipped;
		do
		{
			skipped = token_t();
			for (array<symbol_t*>::iterator i = ignore.begin(); i != ignore.end() && !skipped; i++)
				skipped = (*i)->parse(curr->curr.token.end(), &(curr->curr.context));

			if (skipped)
				curr->curr.token.skip(skipped);
		} while (skipped);

		if (curr->next())
		{
			graph::links next = curr->node.next();
			if (next.size() == 1)
				curr->node = next[0];
			else if (next.size() > 1)
				stack.push_back(syntax_t::iterator(type, next.begin(), curr->curr.token.end()));
			else
			{
				token_t result(type);
				for (array<syntax_t::iterator>::iterator i = stack.begin(); i != stack.end(); i++)
				{
					result.extend(i->curr.token);
					if (context)
						context->extend(i->curr.context);
				}
				return result;
			}
		}
		else
		{
			while (!curr->next_branch())
			{
				array<syntax_t::iterator>::iterator prev = curr-1;
				if (prev)
				{
					prev->curr.token.extend(curr->closest.token);
					prev->curr.context.extend(curr->closest.context);
					stack.drop_back();
					curr = prev;
				}
				else
				{
					if (context)
						context->extend(curr->closest.context);
					return curr->closest.token;
				}
			}
		}
	}

	return token_t(type);
}

