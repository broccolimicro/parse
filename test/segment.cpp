#include <gtest/gtest.h>

#include <parse/segment.h>

using namespace core;

TEST(segment, load)
{
	segment seg;
	seg.load("test/segment.in");
	EXPECT_EQ(seg.name, "test/segment.in");
	EXPECT_EQ(seg.lines.size(), 5);
	EXPECT_EQ(seg.lines[0], string("*[L?v; R!v; R!v]\n"));
	EXPECT_EQ(seg.lines[1], string("\n"));
	EXPECT_EQ(seg.lines[2], string("*[L!rand(0,1)] ||\n"));
	EXPECT_EQ(seg.lines[3], string("*[R?]\n"));
	EXPECT_EQ(seg.lines[4], string("\n"));

	for (segment::iterator i = seg.rbegin(); i; i-=3)
	{
		printf("%d:'%c'\n", *i, *i);
	}
}

