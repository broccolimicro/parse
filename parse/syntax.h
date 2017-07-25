#pragma once

#include <std/graph.h>

#include "symbol.h"
#include "context.h"

struct syntax_t : symbol_t
{
	typedef core::graph<symbol_t*> graph;

	struct iterator
	{
		iterator();
		iterator(string type, graph::iterator node, segment::iterator source);
		iterator(string type, graph::link_iterator branch, segment::iterator source);
		~iterator();

		struct {
			token_t token;
			context_t context;
		} closest, curr;

		graph::iterator node;
		graph::link_iterator branch;

		bool next();
		bool next_branch();
	};

	syntax_t();
	syntax_t(string type);
	~syntax_t();

	graph syntax;
	array<symbol_t*> ignore;
	graph::links start;

	token_t parse(segment::iterator source, context_t *context = NULL);
};

