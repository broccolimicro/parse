/*
 * token.cpp
 *
 *  Created on: May 15, 2017
 *      Author: nbingham
 */

#include "token.h"

token_t::token_t()
{
}

token_t::token_t(string type)
{
	this->type = type;
}

token_t::token_t(string type, segment::iterator start)
{
	this->type = type;
	source.start = start;
	source.finish = start;
}

token_t::~token_t()
{
}

token_t::operator bool()
{
	return source.start && source.finish && source.start != source.finish;
}

segment::iterator &token_t::begin()
{
	return source.start;
}

segment::iterator &token_t::end()
{
	return source.finish;
}

char token_t::peek(int off)
{
	if (source.finish+off)
		return *(source.finish+off);
	else
		return '\0';
}

char token_t::inc(int off)
{
	if (source.finish)
	{
		char result = *source.finish;
		source.finish += off;
		return result;
	}
	else
		return '\0';
}

void token_t::skip(token_t t)
{
	if (!source.finish || t.source.finish > source.finish)
		source.finish = t.source.finish;
	if (!source.start || t.source.start < source.start)
		source.start = t.source.start;
}

void token_t::skip(array<token_t> t)
{
	for (array<token_t>::iterator i = t.begin(); i != t.end(); i++)
		skip(*i);
}

void token_t::append(token_t t)
{
	if (!source.finish || t.source.finish > source.finish)
		source.finish = t.source.finish;
	if (!source.start || t.source.start < source.start)
		source.start = t.source.start;
	tokens.push_back(t);
}

void token_t::append(array<token_t> t)
{
	for (array<token_t>::iterator i = t.begin(); i != t.end(); i++)
		append(*i);
}

void token_t::extend(token_t t)
{
	if (!source.finish || t.source.finish > source.finish)
		source.finish = t.source.finish;
	if (!source.start || t.source.start < source.start)
		source.start = t.source.start;
	tokens.append_back(t.tokens);
}

void token_t::emit(string tab)
{
	printf("%s%s: \"%s\"\n", tab.c_str(), type.c_str(), string(source).c_str());
	printf("%s{\n", tab.c_str());
	for (array<token_t>::iterator i = tokens.begin(); i != tokens.end(); i++)
		i->emit(tab + "\t");
	printf("%s}\n", tab.c_str());
}

token_t &token_t::reset()
{
	source.finish = source.start;
	tokens.clear();
	return *this;
}
