#pragma once

#include "message.h"

#include <std/array.h>

/*#define FAIL(msg) fail(__FILE__, __LINE__, msg)
#define ERROR(msg) error(__FILE__, __LINE__, msg)
#define WARN(msg) warn(__FILE__, __LINE__, msg)
#define NOTE(msg) note(__FILE__, __LINE__, msg)
#define ELAB(msg) elab(__FILE__, __LINE__, msg)
*/

struct context
{
	context();
	~context();

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

	void emit();

	void reset();
};


