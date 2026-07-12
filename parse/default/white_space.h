#pragma once

#include "../tokenizer.h"

namespace parse {

struct white_space {
	string debug_name;

	white_space();
	~white_space();

	static token consume(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
};

}
