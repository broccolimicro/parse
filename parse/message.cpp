#include "message.h"

message::message()
{
	line = -1;
	len = -1;
}

message::message(string file, int line, int type, string txt, segment::iterator loc, int len)
{
	this->file = file;
	this->line = line;
	this->type = type;
	this->loc = loc;
	this->len = len;
	this->txt = txt;
}

message::~message()
{
}

string message::typestr()
{
	switch (type)
	{
		case fail: return "fail";
		case error: return "error";
		case warning: return "warning";
		default: return "note";
	}
}

void message::emit()
{
#ifdef DEBUG
	printf("%s:%d ", file.c_str(), line);
#endif

	printf("%s %s: %s\n", loc.report().c_str(), typestr().c_str(), txt.c_str());
	printf("%s", loc.line->c_str());
	printf("%s", loc.pointer(len).c_str());
}
