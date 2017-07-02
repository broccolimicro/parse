#include <gtest/gtest.h>

#include <parse/segment.h>
#include <parse/tokenizer.h>
#include "default.h"

using namespace core;

TEST(tokenizer, load)
{
	segment seg;

	seg.load("test/tokenizer.in");

	tokenizer tok("test", seg.begin());
	tok.ignore(parse::white_space);

	tok.push();
	tok.expect(parse::text);
	tok.push();
	tok.expect(parse::integer);
	tok.push();
	tok.expect(parse::keyword, "Name4");

	tok.pop();
	tok.pop();
	tok.pop();

	tok.emit();

	printf("Start %s: \"%s\"\n", tok.source.type.c_str(), string(tok.source.syntax).c_str());
	for (array<token>::iterator i = tok.source.tokens.begin(); i != tok.source.tokens.end(); i++)
		printf("\tStart %s: \"%s\"\n", i->type.c_str(), string(i->syntax).c_str());
}

