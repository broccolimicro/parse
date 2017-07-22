#pragma once

#include "token.h"
#include "segment.h"

struct symbol
{
	symbol() {}
	virtual ~symbol() {}

	virtual token parse(segment::iterator) = 0;
};

