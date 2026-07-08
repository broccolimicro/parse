#pragma once

#include <common/standard.h>

namespace parse {

struct syntax {
	syntax()
	{
		debug_name = "syntax";
 		valid = false;
		segment_name = "";
		start = -1;
		end = -1;
	}
	virtual ~syntax() {}

	string segment_name;
	int start, end;
	string debug_name;
	bool valid;

	virtual string to_string(string tab = "") const = 0;
	virtual syntax *clone() const = 0;

	template <typename T>
	bool is_a() const {
		return debug_name == T().debug_name;
	}

	template <typename T>
	T &get() {
		return *((T*)this);
	}

	template <typename T>
	const T &get() const {
		return *((const T*)this);
	}
};

}
