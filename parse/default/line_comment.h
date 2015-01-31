/*
 * line_comment.h
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "../parse.h"

#ifndef parse_default_line_comment_h
#define parse_default_line_comment_h

namespace parse
{
struct line_comment
{
	line_comment();
	~line_comment();

	static token consume(tokenizer &tokens);
	static bool is_next(configuration &config, tokenizer &tokens, int i = 1);
};
}

#endif
