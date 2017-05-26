/*
 * expecter.h
 *
 *  Created on: May 25, 2017
 *      Author: nbingham
 */

#include "segment.h"

#pragma once

struct tokenizer;

typedef tokenizer (*parse_fn)(segment::iterator start, const void *data);

struct expecter
{
	expecter(bool required = true);
	~expecter();

	array<pair<parse_fn, const void*> > options;
	bool required;
	
	void expect(parse_fn p, const void *data = NULL);
};

