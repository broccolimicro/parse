/*
 * syntax.h
 *
 *  Created on: Oct 28, 2014
 *      Author: nbingham
 */

#include <common/standard.h>

#ifndef parse_syntax_h
#define parse_syntax_h

namespace parse
{
struct syntax
{
	syntax() { debug_name = "syntax"; valid = false; }
	virtual ~syntax() {}

	string debug_name;
	bool valid;

	virtual string to_string(string tab = "") const = 0;

	template <class type>
	bool is_a()
	{
		return debug_name == type().debug_name;
	}
};
}

#endif
