/*
 * token.h
 *
 *  Created on: May 15, 2017
 *      Author: nbingham
 */

#include <std/slice.h>
#include <std/array.h>
#include "segment.h"

#pragma once

struct token_t
{
	token_t();
	token_t(string type);
	token_t(string type, segment::iterator start);
	~token_t();

	string type;
	slice<range<segment::iterator> > source;
	array<token_t> tokens;
	
	operator bool();
	
	segment::iterator &begin();
	segment::iterator &end();	

	char peek(int off = 0);
	char inc(int off = 1);

	void skip(token_t t);
	void skip(array<token_t> t);

	void append(token_t t);
	void append(array<token_t> t);
	void extend(token_t t);
	
	void emit(string tab = "");

	token_t &reset();
};
