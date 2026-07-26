#pragma once

#include "tokenizer.h"
#include "syntax.h"

#include <memory>
#include <functional>

namespace parse {

struct schema {
	schema();
	~schema();

	string label;

	using Produce = std::function<syntax*(tokenizer&,std::any)>;
	using IsNext = std::function<bool(tokenizer&,int,std::any)>;
	using RegisterSyntax = std::function<void(tokenizer&)>;

	Produce produce;
	IsNext is_next;
	RegisterSyntax register_syntax;

	template <typename T>
	void set() {
		label = "[" + T().debug_name + "]";
		produce = [](tokenizer &tokens, std::any data) -> syntax* {
			T *result = new T();
			result->parse(tokens, data);
			return result;
		};

		is_next = &T::is_next;
		register_syntax = &T::register_syntax;
	}

	template <typename T>
	static schema from() {
		schema result;
		result.set<T>();
		return result;
	}

	void expect(tokenizer &tokens, std::any data={}) const;
	bool found(tokenizer &tokens) const;
	bool empty() const;
};

}
