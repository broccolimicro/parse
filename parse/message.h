/*
 * message.h
 *
 *  Created on: May 26, 2017
 *      Author: nbingham
 */

#include <std/string.h>
#include "segment.h"

#pragma once

struct message
{
	message();
	message(string file, int line, int type, string txt, segment::iterator loc, int len = 1);
	~message();

	enum {
		fail = 0,
		error = 1,
		warning = 2,
		note = 3,
	};

	string file;
	int line;
	int type;
	string txt;
	segment::iterator loc;
	int len;

	string typestr();
	void emit();
};

