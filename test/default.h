/*
 *  default.h
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include <parse/segment.h>
#include <parse/symbol.h>

#pragma once

namespace parse
{

struct whitespace : symbol_t
{
	whitespace();
	~whitespace();
	token_t parse(segment::iterator source, context_t *context = NULL);
};

struct text : symbol_t
{
	text();
	~text();

	token_t parse(segment::iterator source, context_t *context = NULL);
};

struct integer : symbol_t
{
	integer();
	~integer();
	
	token_t parse(segment::iterator source, context_t *context = NULL);
};

struct real : symbol_t
{
	real();
	~real();

	token_t parse(segment::iterator source, context_t *context = NULL);
};

struct instance : symbol_t
{
	instance();
	~instance();

	token_t parse(segment::iterator source, context_t *context = NULL);
};

struct line_comment : symbol_t
{
	line_comment();
	~line_comment();
	
	token_t parse(segment::iterator source, context_t *context = NULL);
};

struct block_comment : symbol_t
{
	block_comment();
	~block_comment();

	token_t parse(segment::iterator source, context_t *context = NULL);
};

struct keyword : symbol_t
{
	keyword();
	keyword(string value);
	~keyword();

	string value;

	token_t parse(segment::iterator source, context_t *context = NULL);
};

}
