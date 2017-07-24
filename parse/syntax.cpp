#include "syntax.h"

syntax::syntax()
{
}

syntax::syntax(string type)
{
	this->type = type;
}

syntax::~syntax()
{
}

token syntax::parse(segment::iterator seg)
{
	struct elem
	{
		elem() : tok("dummy", seg)
		{
			this->def = start;
		}
		~elem() {}

		token tok;
		syntax::iterator def;
	};

	array<elem> stack;
	stack.push_back(elem());

	while (stack.size() > 0)
	{
		elem *curr = &stack.back();
		token result = *(curr->def)->parse(curr->tok.end());
		if (result)
		{
			curr->tokens.push_back(result);
			curr->tokens.seg = result.
		}
	}

	return token();
}

