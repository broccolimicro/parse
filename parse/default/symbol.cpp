/*
 * symbol.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "symbol.h"

namespace parse
{
symbol::symbol()
{
	debug_name = "symbol";
}

symbol::~symbol()
{

}

token symbol::consume(tokenizer &tokens, void *data)
{
	token result;
	result.type = tokens.token_type<symbol>();
	result.start = tokens.offset+1;

	string one(1, tokens.next_char());
	string two = one + string(1, tokens.peek_char(1));
	string three = two + string(1, tokens.peek_char(2));

	if (three == ">>=" || three == "<<=")
	{
		tokens.next_char();
		tokens.next_char();
	}
	else if (two == ":=" || two == "->" || two == "==" || two == "~=" ||
		two == "<=" || two == ">=" || two == "&&" || two == "||" ||
		two == "**" || two == ">>" || two == "<<" || two == "[]" ||
		two == ".." || two == "*[" || two == "!=" || two == "::" ||
		two == "+=" || two == "-=" || two == "*=" || two == "/=" ||
		two == "&=" || two == "|=" || two == "^=" || two == "++" ||
		two == "--" || two == "%=")
	{
		tokens.next_char();
	}

	result.end = tokens.offset+1;
	return result;
}

bool symbol::is_next(tokenizer &tokens, int i, void *data)
{
	char character = tokens.peek_char(i);

	return (character == '~' || character == '!' || character == '@' || character == '#' ||
			 character == '$' || character == '%' || character == '^' || character == '&' ||
			 character == '*' || character == '(' || character == ')' || character == '-' ||
			 character == '=' || character == '+' || character == '[' || character == '{' ||
			 character == ']' || character == '}' || character == '\\' || character == '|' ||
			 character == ';' || character == ':' || character == ',' || character == '<' ||
			 character == '.' || character == '>' || character == '/' || character == '?');
}

}
