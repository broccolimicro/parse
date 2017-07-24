#pragma once

#include "message.h"

#include <std/array.h>

struct context_t
{
	context_t();
	~context_t();

	array<message> msgs;
	int failures;
	int errors;
	int warnings;
	int notes;

	operator bool();
	
	void fail(string file, int line, string msg, segment::iterator start, int len = 1);
	void error(string file, int line, string msg, segment::iterator start, int len = 1);
	void warn(string file, int line, string msg, segment::iterator start, int len = 1);
	void note(string file, int line, string msg, segment::iterator start, int len = 1);
	void elab(string file, int line, string msg);

	void extend(context_t ctx);

	void emit();

	void reset();
};


