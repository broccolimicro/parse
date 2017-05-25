#include <gtest/gtest.h>

#include <parse/segment.h>

using namespace core;

TEST(segment, load)
{
	segment seg;
	seg.load("test/stream.chp");
	EXPECT_EQ(seg.name, "test/stream.chp");
	EXPECT_EQ(seg.lines.size(), 6);
	EXPECT_EQ(seg.lines[0], string("*[L?v; R!v; R!v]\n"));
	EXPECT_EQ(seg.lines[1], string("\n"));
	EXPECT_EQ(seg.lines[2], string("*[L!rand(0,1)] ||\n"));
	EXPECT_EQ(seg.lines[3], string("*[R?]\n"));
	EXPECT_EQ(seg.lines[4], string("\n"));
	EXPECT_EQ(seg.lines[5], string(""));
}

TEST(segment, iterate)
{
	segment seg;
	seg.load("test/stream.chp");
	segment::iterator i = seg.begin();	
	while (i != seg.end())
	{
		i.warn("Message!", 3);
		i++;
	}
	printf("\n");
}

