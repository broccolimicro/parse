#pragma once

#include "../tokenizer.h"

namespace parse {

struct line_comment {
	string debug_name;

	line_comment();
	~line_comment();

	static token consume(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
};

}
