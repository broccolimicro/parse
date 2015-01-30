/*
 * block_comment.h
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "../parse.h"

#ifndef block_comment_h
#define block_comment_h

namespace parse
{
struct block_comment
{
	block_comment();
	~block_comment();

	static token consume(tokenizer &tokens);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
};
}

#endif
