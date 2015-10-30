/*
 * white_space.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "white_space.h"

namespace parse
{
white_space::white_space()
{
	debug_name = "white_space";
}

white_space::~white_space()
{

}

token white_space::consume(tokenizer &tokens, void *data)
{
	token result;
	result.type = tokens.token_type<white_space>();
	result.start = tokens.offset+1;

	tokens.next_char();

	while (tokens.peek_char(1) == '\t' || tokens.peek_char(1) == ' ')
		tokens.next_char();

	result.end = tokens.offset;
	return result;
}

bool white_space::is_next(tokenizer &tokens, int i, void *data)
{
	return (tokens.peek_char(i) == '\t' || tokens.peek_char(i) == ' ');
}
}
