#pragma once

#include "syntax.h"

namespace parse {

template <typename T>
struct wrapper : parse::syntax {
	std::string value;

	wrapper() {
		this->debug_name = T().debug_name+"_syntax";
	}

	wrapper(std::string value) {
		this->value = value;
		this->debug_name = T().debug_name+"_syntax";
	}

	wrapper(tokenizer &tokens, std::any data) {
		this->debug_name = T().debug_name+"_syntax";
		parse(tokens, data);
	}

	~wrapper() {
	}

	void parse(tokenizer &tokens, std::any data={}) {
		tokens.syntax_start(this);
		
		tokens.increment(true);
		tokens.expect<T>();

		if (tokens.decrement(__FILE__, __LINE__)) {
			value = tokens.next();
		}

		tokens.syntax_end(this);
	}

	static bool is_next(tokenizer &tokens, int i=1, std::any data={}) {
		return tokens.is_next<T>(i);
	}

	static void register_syntax(tokenizer &tokens) {
		if (!tokens.syntax_registered<wrapper<T>>()) {
			tokens.register_syntax<wrapper<T>>();
			tokens.register_token<T>();
		}
	}

	string to_string(string tab="") const {
		return value;
	}

	parse::syntax *clone() const {
		return new wrapper<T>(*this);
	}
};

}

