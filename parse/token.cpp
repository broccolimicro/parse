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

char token::peek(int off)
{
	if (parsed.syntax.finish+off)
		return *(parsed.syntax.finish+off);
	else
		return '\0';
}

char token::inc(int off)
{
	if (parsed.syntax.finish)
	{
		char result = *parsed.syntax.finish;
		parsed.syntax.finish += off;
		return result;
	}
	else
		return '\0';
}

void token::append(token t)
{
	
}

void token::append(array<token> t)
{
	for (array<token>::iterator i = t.begin(); i != t.end(); i++)
		append(*t);
}

void token::emit(string tab)
{
	printf("%s%s: \"%s\"\n", tab.c_str(), type.c_str(), string(syntax).c_str());
	printf("%s{\n", tab.c_str());
	for (array<token>::iterator i = tokens.begin(); i != tokens.end(); i++)
		i->emit(tab + "\t");
	printf("%s}\n", tab.c_str());
}

token &token::reset()
{
	syntax.finish = syntax.start;
	failures = 0;
	errors = 0;
	warnings = 0;
	notes = 0;
	return *this;
}
