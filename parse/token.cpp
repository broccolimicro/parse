/*
 * token.cpp
 *
 *  Created on: May 15, 2017
 *      Author: nbingham
 */

#include "token.h"

token::token()
{
}

token::token(string type)
{
	this->type = type;
}

token::token(string type, segment::iterator start)
{
	this->type = type;
	syntax.start = start;
	syntax.finish = start;
}

token::~token()
{
}

token::operator bool()
{
	return syntax.start && syntax.finish && syntax.start != syntax.finish;
}

segment::iterator &token::begin()
{
	return syntax.start;
}

segment::iterator &token::end()
{
	return syntax.finish;
}


