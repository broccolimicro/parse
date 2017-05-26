/*
 *  default.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "default.h"
#include "error.h"

namespace parse
{

tokenizer white_space(segment::iterator start, const void *data)
{
	tokenizer result("whitespace", start);

	char c = result.peek();
	while (c == ' ' || c == '\t' || c == '\n' || c == '\r')
	{		
		result.inc();
		c = result.peek();
	}

	return result;
}

tokenizer text(segment::iterator start, const void *data)
{
 	tokenizer result("text", start);

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
				ERROR("dangling string expression");
				return result;
			}
		}
	}

	return result;
}

tokenizer dec_integer(segment::iterator start, const void *data)
{
	tokenizer result("decimal integer", start);

	char c = result.peek();
	while (c >= '0' && c <= '9')
	{
		result.inc();
		c = result.peek();
	}

	return result;
}

tokenizer hex_integer(segment::iterator start, const void *data)
{
	tokenizer result("hexidecimal integer", start);

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

tokenizer oct_integer(segment::iterator start, const void *data)
{
	tokenizer result("octal integer", start);

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

tokenizer bin_integer(segment::iterator start, const void *data)
{
	tokenizer result("binary integer", start);

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

tokenizer integer(segment::iterator start, const void *data)
{
	if (start && *start == '0')
	{
		char base = *(start+1);
		switch (base)
		{
		case 'x': return hex_integer(start, data);
		case 'o': return oct_integer(start, data);
		case 'b': return bin_integer(start, data);
		default: return dec_integer(start, data);
		}
	}
	else
		return dec_integer(start, data);
}

tokenizer real(segment::iterator start, const void *data)
{
	tokenizer result("real", start);

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
			ERROR("missing exponent in real");

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
		return result.reset();
	else
		return result;
}

tokenizer instance(segment::iterator start, const void *data)
{
	tokenizer result("instance", start);

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

tokenizer line_comment(segment::iterator start, const void *data)
{
	tokenizer result("line comment", start);

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

tokenizer block_comment(segment::iterator start, const void *data)
{
	tokenizer result("block comment", start);

	if (result.peek() == '/' && result.peek(1) == '*')
	{
		result.inc(2);

		char c0 = result.peek(), c1 = result.peek(1);
		while (c0 != '*' || c1 != '/')
		{
			if (c0 == '\0')
			{
				ERROR("dangling block comment");
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

tokenizer keyword(segment::iterator start, const void *data)
{
	tokenizer result("keyword", start);
	
	for (const char *k = (const char*)data; *k; k++)
	{
		if (*k != result.peek())
			return result.reset();

		result.inc();
	}

	return result;
}

}
