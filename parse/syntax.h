#pragma once

#include <std/graph.h>

#include "symbol.h"

struct syntax : symbol
{
	syntax();
	~syntax();

	core::graph<symbol*> def;
	core::graph<symbol*>::iterator start;

	token parse(segment::iterator start);
};

