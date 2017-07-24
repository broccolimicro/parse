#include <gtest/gtest.h>

#include <std/hash_map.h>

#include <parse/segment.h>
#include <parse/syntax.h>
#include "default.h"

using namespace core;

TEST(syntax, load)
{
	segment seg;

	seg.load("test/syntax.in");

	hash_map<string, symbol_t*> symbols;
	symbols.insert("whitespace", new parse::whitespace());
	symbols.insert("text", new parse::text());
	symbols.insert("integer", new parse::integer());
	symbols.insert("real", new parse::real());
	symbols.insert("instance", new parse::instance());
	symbols.insert("line comment", new parse::line_comment());
	symbols.insert("block comment", new parse::block_comment());
	symbols.insert("keyword", new parse::keyword("Name4"));
	
	syntax_t test("test");
	syntax_t::graph::iterator cur;
	cur = test.syntax.insert(symbols["keyword"]);
	test.start = cur;
	cur = cur.push(symbols["integer"]);
	cur = cur.push(symbols["text"]);

	test.ignore.push_back(symbols["whitespace"]);
	test.ignore.push_back(symbols["line comment"]);
	test.ignore.push_back(symbols["block comment"]);

	context_t context;
	token_t result;

	result = test.parse(seg.begin(), &context);

	result.emit();
	context.emit();
}

