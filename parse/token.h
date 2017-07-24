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

struct token
{
	token();
	token(string type);
	token(string type, segment::iterator start);
	~token();

	string type;
	slice<range<segment::iterator> > syntax;
	array<token> tokens;
	
	operator bool();
	
	segment::iterator &begin();
	segment::iterator &end();	

	char peek(int off = 0);
	char inc(int off = 1);

	void append(token t);
	void append(array<token> t);
	
	void emit(string tab = "");

	token &reset();
};
