#pragma once

#include "parse.h"

namespace parse {

struct factory {
	typedef void (*Register)(tokenizer&);
	typedef void (*Expect)(tokenizer&);
	typedef parse::syntax *(*Produce)(tokenizer&, void*);

	const Register register_syntax;
	const Expect expect;
	const Produce produce;
	void *data;

	factory();
	factory(const Register register_syntax, const Expect expect, const Produce produce, void *data=nullptr);
	~factory();

	operator bool() const;
};

struct registry {
	virtual const factory *getParser(std::string lang) const = 0;
	virtual std::vector<std::string> getParserIndex() const = 0;
};

}
