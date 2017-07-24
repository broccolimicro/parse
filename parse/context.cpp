#include "context.h"

context_t::context_t()
{
	failures = 0;
	errors = 0;
	warnings = 0;
	notes = 0;
}

context_t::~context_t()
{
}

context_t::operator bool()
{
	return failures == 0 && errors == 0;
}

void context_t::fail(string file, int line, string msg, segment::iterator start, int len)
{
	failures++;
	msgs.push_back(message(file, line, message::fail, msg, start, len));
}

void context_t::error(string file, int line, string msg, segment::iterator start, int len)
{
	errors++;
	msgs.push_back(message(file, line, message::error, msg, start, len));
}

void context_t::warn(string file, int line, string msg, segment::iterator start, int len)
{
	warnings++;
	msgs.push_back(message(file, line, message::warning, msg, start, len));
}

void context_t::note(string file, int line, string msg, segment::iterator start, int len)
{
	notes++;
	msgs.push_back(message(file, line, message::note, msg, start, len));
}

void context_t::elab(string file, int line, string msg)
{
	if (msgs.size() > 0)
		msgs.back().txt += "\n" + msg;
	else
		fail(file, line, "no message to elaborate", segment::iterator());
}

void context_t::extend(context_t ctx)
{
	msgs.append_back(ctx.msgs);
	failures += ctx.failures;
	errors += ctx.errors;
	warnings += ctx.warnings;
	notes += ctx.notes;
}

void context_t::emit()
{
	for (array<message>::iterator m = msgs.begin(); m != msgs.end(); m++)
		m->emit();
}

void context_t::reset()
{
	failures = 0;
	errors = 0;
	warnings = 0;
	notes = 0;
}
