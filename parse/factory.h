#pragma once

#include <any>

#include "schema.h"

namespace parse {

struct factory {
	schema sub;
	std::any data;

	factory();
	factory(schema sub, std::any data={});
	~factory();

	void register_syntax(tokenizer &tokens) const;
	void expect(tokenizer &tokens) const;
	parse::syntax *produce(tokenizer &tokens) const;
	bool is_next(tokenizer &tokens, int i) const;
	bool found(tokenizer &tokens) const;
	bool empty() const;
};

struct registry {
	virtual const factory *getParser(std::string lang) const = 0;
	virtual std::vector<std::string> getParserIndex() const = 0;
};

}

