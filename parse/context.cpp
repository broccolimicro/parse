#include "context.h"

context::context()
{
	failures = 0;
	errors = 0;
	warnings = 0;
	notes = 0;
}

context::~context()
{
}

context::operator bool()
{
	return failures == 0 && errors == 0;
}

void context::fail(string file, int line, string msg, int len)
{
	failures++;
	msgs.push_back(message(file, line, message::fail, msg, parsed.end(), len));
}

void context::error(string file, int line, string msg, int len)
{
	errors++;
	msgs.push_back(message(file, line, message::error, msg, parsed.end(), len));
}

void context::warn(string file, int line, string msg, int len)
{
	warnings++;
	msgs.push_back(message(file, line, message::warning, msg, parsed.end(), len));
}

void context::note(string file, int line, string msg, int len)
{
	notes++;
	msgs.push_back(message(file, line, message::note, msg, parsed.end(), len));
}

void context::elab(string file, int line, string msg)
{
	if (msgs.size() > 0)
		msgs.back().txt += "\n" + msg;
	else
		fail(file, line, "no message to elaborate");
}

void context::emit()
{
	for (array<message>::iterator m = msgs.begin(); m != msgs.end(); m++)
		m->emit();
}

void context::reset()
{
	failures = 0;
	errors = 0;
	warnings = 0;
	notes = 0;
}
