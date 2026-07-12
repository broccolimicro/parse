#pragma once

#include "../tokenizer.h"

namespace parse {

struct new_line {
	string debug_name;

	new_line();
	~new_line();

	static token consume(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
};

}
