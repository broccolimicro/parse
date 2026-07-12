#pragma once

#include "../tokenizer.h"

namespace parse {

struct symbol {
	string debug_name;

	symbol();
	~symbol();

	static token consume(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
};

}
