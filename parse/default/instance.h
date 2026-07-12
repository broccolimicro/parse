#pragma once

#include "../tokenizer.h"

namespace parse {

struct instance {
	string debug_name;

	instance();
	~instance();

	static token consume(tokenizer &tokens, std::any data=std::any());
	static bool is_next(tokenizer &tokens, int i = 1, std::any data=std::any());
};

}
