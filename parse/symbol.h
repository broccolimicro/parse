#pragma once

#include "token.h"
#include "segment.h"
#include "context.h"

struct symbol_t
{
	symbol_t() {}
	virtual ~symbol_t() {}

	string type;

	virtual token_t parse(segment::iterator source, context_t *context = NULL) = 0;
};

