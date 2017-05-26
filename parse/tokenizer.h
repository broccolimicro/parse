/*
 * tokenizer.h
 *
 *  Created on: May 25, 2017
 *      Author: nbingham
 */

#include "token.h"
#include "expecter.h"
#include "message.h"

#pragma once

struct tokenizer
{
	tokenizer();
	tokenizer(string name, segment::iterator start);
	~tokenizer();

	typedef array<expecter>::iterator iterator;

	expecter skipped;
	array<expecter> stack;
	token source;

	array<message> msgs;
	int failures;
	int errors;
	int warnings;
	int notes;

	operator bool();

	void fail(string file, int line, string msg, int len = 1);
	void error(string file, int line, string msg, int len = 1);
	void warn(string file, int line, string msg, int len = 1);
	void note(string file, int line, string msg, int len = 1);
	void elab(string file, int line, string msg);

	iterator push(bool required = true);
	iterator push(expecter e);
	bool pop();
	
	void expect(parse_fn p, const void *data = NULL);
	void ignore(parse_fn p, const void *data = NULL);

	bool next(tokenizer *t);

	string expected_string();

	char peek(int off = 0);
	char inc(int off = 1);
	void inc(token t);
	void inc(const tokenizer &t);
	void skip(token t);
	void skip(const tokenizer &t);
	void skip();
	void emit();

	tokenizer &reset();
};

