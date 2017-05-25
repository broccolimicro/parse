/*
 * segment.h
 *
 *  Created on: Oct 15, 2014
 *      Author: nbingham
 */

#include <std/string.h>
#include "segment.h"

#pragma once

struct tokenizer
{
	tokenizer();
	tokenizer(segment *seg);
	~tokenizer();
	
	segment::iterator curr;	
};
