#pragma once

#include <std/graph.h>

#include "symbol.h"

struct syntax : symbol
{
	typedef iterator core::graph<symbol*>::iterator;

	syntax();
	syntax(string type);
	~syntax();

	string type;
	core::graph<symbol*> def;
	iterator start;

	token parse(segment::iterator seg);
};

