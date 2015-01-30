/*
 * number.h
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "../parse.h"

#ifndef number_h
#define number_h

namespace parse
{
struct number
{
	number();
	~number();

	string debug_name;

	static token consume(tokenizer &tokens);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
};
}

#endif
