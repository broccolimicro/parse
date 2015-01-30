/*

 * common.h
 *
 * Common is a collection of functions not specific to the compiler that
 * we found useful to define. Note that our #defines for user flags are
 * also stored in common.h as it is accessed by everyone.
 */

#ifndef common_h
#define common_h

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <sstream>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <iterator>

using namespace std;

bool ac(char c);
bool nc(char c);
bool oc(char c);
bool sc(char c);

template <class type>
string to_string(type value)
{
	ostringstream os;
	os << value;
	return os.str();
}

#endif
