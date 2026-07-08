#pragma once

#include "parse.h"
#include "syntax.h"

#include <memory>
#include <functional>

namespace parse {

struct schema {
	schema();
	~schema();

	string label;

	std::function<std::shared_ptr<syntax>(tokenizer&,void*)> factory;
	std::function<bool(tokenizer&,int,void*)> is_next;
	std::function<void(tokenizer&)> register_syntax;

	template <typename T>
	void set() {
		label = "[" + T().debug_name + "]";
		factory = [](tokenizer &tokens, void *data) -> std::shared_ptr<syntax> {
			std::shared_ptr<syntax> result = std::make_shared<T>();
			((T*)result.get())->parse(tokens, data);
			return result;
		};

		is_next = &T::is_next;
		register_syntax = &T::register_syntax;
	}

	bool empty() const;
};

}
