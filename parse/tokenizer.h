/*
 * tokenizer.h
 *
 *  Created on: Apr 7, 2014
 *      Author: nbingham
 */

#include "common.h"
#include "segment.h"
#include "token.h"

#ifndef tokenizer_h
#define tokenizer_h

struct tokenizer;
struct configuration;

typedef bool (*is_next_ptr)(configuration&, tokenizer&, int);
typedef token (*consume_ptr)(tokenizer&);

struct token_entry
{
	token_entry(is_next_ptr n, consume_ptr c, int t)
	{
		is_next = n;
		consume = c;
		type = t;
	}

	~token_entry(){}

	is_next_ptr is_next;
	consume_ptr consume;
	int type;
};

/** This structure splits up multiple input files onto a stream of
 *  tokens and then facilitates search and iteration though those
 *  tokens and handling errors.
 */
struct tokenizer
{
	tokenizer();
	~tokenizer();

	map<string, is_next_ptr> syntax_registry;
	typedef typename map<string, is_next_ptr>::iterator syntax_registry_iterator;
	map<string, token_entry> token_registry;
	typedef typename map<string, token_entry>::iterator token_registry_iterator;
	vector<pair<is_next_ptr, consume_ptr> > comment_registry;
	vector<segment> segments;
	vector<int> index;
	int offset;
	int segment_index;

	vector<pair<vector<string>, bool> > expected_hierarchy;
	string found_type;

	template <class type>
	void register_syntax()
	{
		syntax_registry.insert(pair<string, is_next_ptr>("[" + type().debug_name + "]", &type::is_next));
	}

	template <class type>
	bool syntax_registered()
	{
		return (syntax_registry.find("[" + type().debug_name + "]") != syntax_registry.end());
	}

	template <class type>
	void register_comment()
	{
		comment_registry.push_back(pair<is_next_ptr, consume_ptr>(&type::is_next, &type::consume));
	}

	template <class type>
	bool comment_registered()
	{
		return (find(comment_registry.begin(), comment_registry.end(), pair<is_next_ptr, consume_ptr>(&type::is_next, &type::consume)) != comment_registry.end());
	}

	template <class type>
	int comment_type()
	{
		return (find(comment_registry.begin(), comment_registry.end(), pair<is_next_ptr, consume_ptr>(&type::is_next, &type::consume)) - comment_registry.begin());
	}

	template <class type>
	void register_token()
	{
		string name = "[" + type().debug_name + "]";
		if (token_registry.find(name) == token_registry.end())
			token_registry.insert(pair<string, token_entry>(name, token_entry(&type::is_next, &type::consume, (int)token_registry.size())));
	}

	template <class type>
	bool token_registered()
	{
		return (token_registry.find("[" + type().debug_name + "]") != token_registry.end());
	}

	template <class type>
	int token_type()
	{
		return token_registry.find("[" + type().debug_name + "]")->second.type;
	}

	void increment(bool required = true);
	bool decrement(configuration &config, string debug_file, int debug_line);

	void expect(string s);
	void expect(vector<string> &s);
	template <class type>
	void expect()
	{
		if (!syntax_registered<type>() && !token_registered<type>())
			internal(*this, "syntax or token not registered \"" + type().debug_name + "\"", __FILE__, __LINE__);
		expected_hierarchy.back().first.push_back("[" + type().debug_name + "]");
	}
	
	pair<int, int> expected(configuration &config, int off = 1);
	pair<int, int> expected(string s);

	int number_expected(int required_index);

	bool found(string s);
	template <class type>
	bool found()
	{
		return (found_type == ("[" + type().debug_name + "]"));
	}

	void insert(configuration &config, string name, string contents);

	bool is_next(string str, int i = 1);
	template <class type>
	bool is_next(int i = 1)
	{
		return (peek_type(i) == "[" + type().debug_name + "]");
	}

	string file(int i = 0);
	string line(int i = 0);
	string location(int s, int o);
	int line_number(int i = 0);
	int line_offset(int i = 0);
	int segment_offset(int i = 0);

	char curr_char();
	char prev_char();
	char next_char();
	char peek_char(int i);

	string curr_type();
	string curr();
	string prev();
	string next();
	string peek_type(int i);
	string peek(int i);

	bool segment_loaded(string name);
	bool segment_loading(string name);
};

#endif
