/*
 * tokenizer.h
 *
 *  Created on: May 25, 2017
 *      Author: nbingham
 */

#include "tokenizer.h"
#include <std/fill.h>

#define FAIL(msg) fail(__FILE__, __LINE__, msg)
#define ERROR(msg) error(__FILE__, __LINE__, msg)
#define WARN(msg) warn(__FILE__, __LINE__, msg)
#define NOTE(msg) note(__FILE__, __LINE__, msg)
#define ELAB(msg) elab(__FILE__, __LINE__, msg)

tokenizer::tokenizer()
{
	failures = 0;
	errors = 0;
	warnings = 0;
	notes = 0;
}

tokenizer::tokenizer(string name, segment::iterator start) : source(name, start)
{
	failures = 0;
	errors = 0;
	warnings = 0;
	notes = 0;
}

tokenizer::~tokenizer()
{
}

tokenizer::operator bool()
{
	return failures == 0 && errors == 0 && source;
}

void tokenizer::fail(string file, int line, string msg, int len)
{
	failures++;
	msgs.push_back(message(file, line, message::fail, msg, source.end(), len));
}

void tokenizer::error(string file, int line, string msg, int len)
{
	errors++;
	msgs.push_back(message(file, line, message::error, msg, source.end(), len));
}

void tokenizer::warn(string file, int line, string msg, int len)
{
	warnings++;
	msgs.push_back(message(file, line, message::warning, msg, source.end(), len));
}

void tokenizer::note(string file, int line, string msg, int len)
{
	notes++;
	msgs.push_back(message(file, line, message::note, msg, source.end(), len));
}

void tokenizer::elab(string file, int line, string msg)
{
	if (msgs.size() > 0)
		msgs.back().txt += "\n" + msg;
	else
		fail(file, line, "no message to elaborate");
}

tokenizer::iterator tokenizer::push(bool required)
{
	stack.push_back(expecter(required));
	return stack.rbegin();
}

tokenizer::iterator tokenizer::push(expecter e)
{
	stack.push_back(e);
	return stack.rbegin();
}

bool tokenizer::pop()
{
	static int tab_count = 0;
	string tab(fill<char>(tab_count, '\t'));

	printf("%senter tokenizer::pop()\n", tab.c_str());
	printf("%svalue: \"%s\"\n", tab.c_str(), string(source.syntax).c_str());

	tab_count++;
	skip();
	tab_count--;

	printf("%safter skip()\n", tab.c_str());
	printf("%svalue: \"%s\"\n", tab.c_str(), string(source.syntax).c_str());

	tab_count++;

	tokenizer result;
	bool top = next(&result);
	
	tab_count--;

	printf("%snext() %d\n", tab.c_str(), (bool)result);
	printf("%svalue: \"%s\"\n", tab.c_str(), string(result.source.syntax).c_str());

	if (!result && stack.back().required)
	{
		ERROR("expected " + expected_string());
		msgs.append_back(result.msgs);
		failures += result.failures;
		errors += result.errors;
		warnings += result.warnings;
		notes += result.notes;

		tab_count++;
		// to reduce spurious errors, we move ahead
		// until we find what we are looking for
		while (!result && source.end())
		{
			inc();
			top = next(&result);
		}

		tab_count--;
	}

	printf("%safter !result\n", tab.c_str());
	printf("%svalue: \"%s\"\n", tab.c_str(), string(source.syntax).c_str());

	stack.drop_back();
	if (result && top)
		inc(result);

	printf("%sexit tokenizer::pop()\n", tab.c_str());
	printf("%svalue: \"%s\"\n", tab.c_str(), string(source.syntax).c_str());

	return top;
}

bool tokenizer::next(tokenizer *result)
{
	array<tokenizer> results;
	bool top = true;
	for (array<expecter>::iterator i = stack.rbegin(); i != stack.rend() && results.size() == 0; i--)
	{
		top = (i == stack.rbegin());

		for (array<pair<parse_fn, const void*> >::iterator j = i->options.begin(); j != i->options.end(); j++)
		{
			tokenizer parsed = j->first(source.end(), j->second);
			printf("parse() %d\n", (bool)parsed);
			printf("value: \"%s\"\n", string(parsed.source.syntax).c_str());

	
			if (top && i->required)
			{
				msgs.append_back(parsed.msgs);
				failures += parsed.failures;
				errors += parsed.errors;
				warnings += parsed.warnings;
				notes += parsed.notes;
			}

			if (parsed)
				results.push_back(parsed);
			else
				parsed.emit();
		}

		if (i->required)
			break;
	}

		
	if (results.size() > 0)
	{
		if (results.size() > 1)
		{
			FAIL("ambiguous grammar");
			for (array<tokenizer>::iterator i = results.begin(); i != results.end(); i++)
				ELAB(string("found ") + i->source.type + ": \"" + string(i->source.syntax) + "\"");
		}

		*result = results[0];
		return top;
	}
	else
	{
		*result = tokenizer();
		return false;
	}
}

void tokenizer::expect(parse_fn p, const void *data)
{
	stack.back().expect(p, data);
}

void tokenizer::ignore(parse_fn p, const void *data)
{
	skipped.expect(p, data);
}

string tokenizer::expected_string()
{
	string result;
	int count = 0;
	for (array<expecter>::iterator i = stack.rbegin(); i != stack.rend(); i--)
	{
		for (array<pair<parse_fn, const void*> >::iterator j = i->options.begin(); j != i->options.end(); j++)
		{
			string name = j->first(segment::iterator(), j->second).source.type;
			if (!i->required || j != i->options.rbegin())
				result += name + ", ";
			else if (count > 0)
				result += "or " + name;
			else
				result += name;

			count++;
		}

		if (i->required)
			break;
	}

	return result;
}

char tokenizer::peek(int off)
{
	if (source.syntax.finish+off)
		return *(source.syntax.finish+off);
	else
		return '\0';
}

char tokenizer::inc(int off)
{
	if (source.syntax.finish)
	{
		char result = *source.syntax.finish;
		source.syntax.finish += off;
		return result;
	}
	else
		return '\0';
}

void tokenizer::inc(token t)
{
	if (t.syntax.finish > source.syntax.finish)
		source.syntax.finish = t.syntax.finish;
	if (t.syntax.start < source.syntax.start)
		source.syntax.start = t.syntax.start;
	source.tokens.push_back(t);
}

void tokenizer::skip(token t)
{
	if (t.syntax.finish > source.syntax.finish)
		source.syntax.finish = t.syntax.finish;
	if (t.syntax.start < source.syntax.start)
		source.syntax.start = t.syntax.start;
}

void tokenizer::inc(const tokenizer &t)
{
	inc(t.source);
	msgs.append_back(t.msgs);
	failures += t.failures;
	errors += t.errors;
	warnings += t.warnings;
	notes += t.notes;
}

void tokenizer::skip(const tokenizer &t)
{
	skip(t.source);
	msgs.append_back(t.msgs);
	failures += t.failures;
	errors += t.errors;
	warnings += t.warnings;
	notes += t.notes;
}

void tokenizer::skip()
{
	tokenizer result;
	do
	{
		result = tokenizer();
		for (array<pair<parse_fn, const void*> >::iterator i = skipped.options.begin(); i != skipped.options.end() && !result; i++)
			result = i->first(source.end(), i->second);

		if (result)
			skip(result);
	} while (result);
}

void tokenizer::emit()
{
	for (array<message>::iterator m = msgs.begin(); m != msgs.end(); m++)
		m->emit();
}

tokenizer &tokenizer::reset()
{
	source.syntax.finish = source.syntax.start;
	stack.clear();
	return *this;
}
