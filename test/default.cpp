/*
 *  default.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "default.h"
#include <parse/error.h>

namespace parse
{

whitespace::whitespace()
{
	type = "whitespace";
}

whitespace::~whitespace()
{
}

token_t whitespace::parse(segment::iterator source, context_t *context)
{
	token_t result("whitespace", source);

	char c = result.peek();
	while (c == ' ' || c == '\t' || c == '\n' || c == '\r')
	{		
		result.inc();
		c = result.peek();
	}

	return result;
}

text::text()
{
	type = "text";
}

text::~text()
{
}

token_t text::parse(segment::iterator source, context_t *context)
{
 	token_t result("text", source);

	char base = result.peek();
	if (base == '\'' || base == '\"')
	{
		result.inc();

		bool escaped = false;
		bool done = false;

		char c;
		while (!done)
		{
			c = result.inc();

			if (escaped)
				escaped = false;
			else if (c == '\\')
				escaped = true;
			else if (c == base)
				done = true;
			else if (c == '\0')
			{
				if (context)
					ERROR(*context, result.end(), "dangling string expression");
				return result;
			}
		}
	}

	return result;
}

token_t dec_integer(segment::iterator source, context_t *context)
{
	token_t result("integer", source);

	char c = result.peek();
	while (c >= '0' && c <= '9')
	{
		result.inc();
		c = result.peek();
	}

	return result;
}

token_t hex_integer(segment::iterator source, context_t *context)
{
	token_t result("integer", source);

	if (result.peek() == '0' && result.peek(1) == 'x')
	{
		result.inc(2);

		char c = result.peek();
		while ((c >= '0' && c <= '9') || 
					 (c >= 'a' && c <= 'f') ||
					 (c >= 'A' && c <= 'F'))
		{
			result.inc();
			c = result.peek();
		}
	}

	return result;
}

token_t oct_integer(segment::iterator source, context_t *context)
{
	token_t result("integer", source);

	if (result.peek() == '0' && result.peek(1) == 'o')
	{
		result.inc(2);

		char c = result.peek();
		while (c >= '0' && c <= '7')
		{
			result.inc();
			c = result.peek();
		}
	}

	return result;
}

token_t bin_integer(segment::iterator source, context_t *context)
{
	token_t result("integer", source);

	if (result.peek() == '0' && result.peek(1) == 'b')
	{
		result.inc(2);

		char c = result.peek();
		while (c >= '0' && c <= '1')
		{
			result.inc();
			c = result.peek();
		}
	}

	return result;
}

integer::integer()
{
	type = "integer";
}

integer::~integer()
{
}

token_t integer::parse(segment::iterator source, context_t *context)
{
	if (source && *source == '0')
	{
		char base = *(source+1);
		switch (base)
		{
		case 'x': return hex_integer(source, context);
		case 'o': return oct_integer(source, context);
		case 'b': return bin_integer(source, context);
		default: return dec_integer(source, context);
		}
	}
	else
		return dec_integer(source, context);
}

real::real()
{
	type = "real";
}

real::~real()
{
}

token_t real::parse(segment::iterator source, context_t *context)
{
	token_t result("real", source);

	char c = result.peek();
	while (c >= '0' && c <= '9')
	{
		result.inc();
		c = result.peek();
	}

	bool decimal = false;
	if (c == '.')
	{
		result.inc();
		c = result.peek();
		while (c >= '0' && c <= '9')
		{
			result.inc();
			c = result.peek();
		}

		decimal = true;
	}

	bool exponent = false;
	if (c == 'e' || c == 'E')
	{
		result.inc();
		c = result.peek();
		if (c == '-')
		{
			result.inc();
			c = result.peek();
		}

		if (c < '0' || c > '9')
			if (context)
				ERROR(*context, result.end(), "missing exponent in real");

		while (c >= '0' && c <= '9')
		{
			result.inc();
			c = result.peek();
		}

		exponent = true;
	}

	// If there is neither a decimal point nor an exponent,
	// then this is an integer instead of a real value
	if (!decimal && !exponent)
	{
		if (context)
			ERROR(*context, result.begin(), "this is an integer, not decimal value");
		return result.reset();
	}
	else
		return result;
}

instance::instance()
{
	type = "instance";
}

instance::~instance()
{
}

token_t instance::parse(segment::iterator source, context_t *context)
{
	token_t result("instance", source);

	char c = result.peek();
	if ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			c == '_')
	{
		result.inc();

		c = result.peek();
		while ((c >= 'a' && c <= 'z') ||
					 (c >= 'A' && c <= 'Z') ||
					 (c >= '0' && c <= '9') ||
					  c == '_')
		{
			result.inc();
			c = result.peek();
		}
	}

	return result;
}

line_comment::line_comment()
{
	type = "line comment";
}

line_comment::~line_comment()
{
}

token_t line_comment::parse(segment::iterator source, context_t *context)
{
	token_t result("line comment", source);

	if (result.peek() == '/' && result.peek(1) == '/')
	{
		result.inc(2);

		char c = result.peek();
		while (c != '\n' && c != '\0')
		{
			result.inc();
			c = result.peek();
		}
	}

	return result;
}

block_comment::block_comment()
{
	type = "block comment";
}

block_comment::~block_comment()
{
}

token_t block_comment::parse(segment::iterator source, context_t *context)
{
	token_t result("block comment", source);

	if (result.peek() == '/' && result.peek(1) == '*')
	{
		result.inc(2);

		char c0 = result.peek(), c1 = result.peek(1);
		while (c0 != '*' || c1 != '/')
		{
			if (c0 == '\0')
			{
				if (context)
					ERROR(*context, result.end(), "dangling block comment");
				return result;
			}

			result.inc();
			c0 = result.peek();
			c1 = result.peek(1);
		}

		result.inc(2);
	}

	return result;
}

keyword::keyword()
{
	type = "keyword";
}

keyword::keyword(string value)
{
	this->type = "keyword";
	this->value = value;
}

keyword::~keyword()
{
}

token_t keyword::parse(segment::iterator source, context_t *context)
{
	token_t result("keyword", source);
	
	for (string::iterator k = value.begin(); k != value.end(); k++)
	{
		if (*k != result.peek())
		{
			if (context)
				ERROR(*context, result.begin(), "expected keyword \"" + value + "\"");
			return result.reset();
		}

		result.inc();
	}

	return result;
}

}
