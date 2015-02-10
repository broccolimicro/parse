/*
 * number.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "number.h"

namespace parse
{
number::number()
{
	debug_name = "number";
}

number::~number()
{

}

token number::consume(tokenizer &tokens, void *data)
{
	token result;
	result.type = tokens.token_type<number>();
	result.start = tokens.offset+1;

	char character = tokens.peek_char(1);
	if (character == '-')
	{
		tokens.next_char();
		character = tokens.peek_char(1);
	}

	while (character >= '0' && character <= '9')
	{
		tokens.next_char();
		character = tokens.peek_char(1);
	}

	if (character == '.')
	{
		tokens.next_char();
		character = tokens.peek_char(1);
		while (character >= '0' && character <= '9')
		{
			tokens.next_char();
			character = tokens.peek_char(1);
		}
	}

	result.end = tokens.offset+1;
	return result;
}

bool number::is_next(tokenizer &tokens, int i, void *data)
{
	if (tokens.peek_char(i) == '-')
		i++;
	if (tokens.peek_char(i) == '.')
		i++;

	return (tokens.peek_char(i) >= '0' && tokens.peek_char(i) <= '9');
}

}
