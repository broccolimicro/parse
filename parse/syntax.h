/*
 * syntax.h
 *
 *  Created on: Oct 28, 2014
 *      Author: nbingham
 */

#include "common.h"

#ifndef syntax_h
#define syntax_h

namespace parse
{
struct syntax
{
	syntax() { debug_name = "syntax"; valid = false; }
	virtual ~syntax() {}

	string debug_name;
	bool valid;

	virtual string to_string(string tab) = 0;

	template <class type>
	bool is_a()
	{
		return debug_name == type().debug_name;
	}
};
}

#endif
