/*
 *  default.h
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include <parse/segment.h>
#include <parse/tokenizer.h>

#pragma once

namespace parse
{

tokenizer white_space(segment::iterator start, const void *data = NULL);
tokenizer text(segment::iterator start, const void *data = NULL);
tokenizer integer(segment::iterator start, const void *data = NULL);
tokenizer real(segment::iterator start, const void *data = NULL);
tokenizer instance(segment::iterator start, const void *data = NULL);
tokenizer line_comment(segment::iterator start, const void *data = NULL);
tokenizer block_comment(segment::iterator start, const void *data = NULL);
tokenizer keyword(segment::iterator start, const void *data = NULL);

}
