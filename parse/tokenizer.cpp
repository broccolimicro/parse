/*
 * tokenizer.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: nbingham
 */

#include "tokenizer.h"
#include "configuration.h"
#include "message.h"
#include "common.h"

tokenizer::tokenizer()
{
	segment_index = -1;
	offset = -1;
}

tokenizer::~tokenizer()
{

}

void tokenizer::increment(bool required)
{
	expected_hierarchy.push_back(pair<vector<string>, bool>(vector<string>(), required));
}

bool tokenizer::decrement(configuration &config, string debug_file, int debug_line)
{
	pair<int, int> idx(-1, -1);

	int max_required_level;
	for (max_required_level = (int)expected_hierarchy.size()-1; max_required_level >= 0 && !expected_hierarchy[max_required_level].second; max_required_level--);

	idx = expected(config, 1);
	if (idx.first < max_required_level && max_required_level == (int)expected_hierarchy.size()-1)
	{
		string error_string = "expected ";
		vector<string> expect_list;
		for (int j = (int)expected_hierarchy.size()-1; j >= max_required_level; j--)
		{
			for (int k = 0; k < (int)expected_hierarchy[j].first.size(); k++)
			{
				if (expected_hierarchy[j].first[k].size() > 2 && expected_hierarchy[j].first[k][0] == '[' && expected_hierarchy[j].first[k][expected_hierarchy[j].first[k].size()-1] == ']')
					expect_list.push_back(expected_hierarchy[j].first[k].substr(1, expected_hierarchy[j].first[k].size()-2));
				else
					expect_list.push_back("'" + expected_hierarchy[j].first[k] + "'");
			}
		}

		if (expect_list.size() > 0)
		error_string += expect_list[0];

		for (int j = 1; j < (int)expect_list.size()-1; j++)
			error_string += " " + expect_list[j];

		if (expect_list.size() > 1)
			error_string += " or " + expect_list.back();

		error(*this, error_string, "", debug_file, debug_line, 1);

		while ((idx = expected(config, 1)).first < max_required_level && next() != "");
	}

	if (idx.first < (int)expected_hierarchy.size() - 1)
		found_type = "";
	else
		found_type = expected_hierarchy[idx.first].first[idx.second];

	expected_hierarchy.pop_back();

	return (found_type != "");
}

void tokenizer::expect(string s)
{
	expected_hierarchy.back().first.push_back(s);
}

void tokenizer::expect(vector<string> &s)
{
	expected_hierarchy.back().first.insert(expected_hierarchy.back().first.end(), s.begin(), s.end());
}

pair<int, int> tokenizer::expected(configuration &config, int i)
{
	vector<pair<int, int> > results;
	if (segment_index >= 0 && segment_index < (int)segments.size())
	{
		for (int j = (int)expected_hierarchy.size()-1; j >= 0 && results.size() == 0; j--)
		{
			vector<string>::iterator j2;
			// Look for raw strings
			for (j2 = expected_hierarchy[j].first.begin(); j2 != expected_hierarchy[j].first.end(); j2++)
				if ((j2->size() <= 2 || j2->at(0) != '[' || j2->at(j2->size()-1) == ']') && segments[segment_index].get_token(index[segment_index] + i) == *j2)
					results.push_back(pair<int, int>(j, j2 - expected_hierarchy[j].first.begin()));

			if (results.size() == 0)
			{
				// Look for specific token types
				token_registry_iterator iter = token_registry.find(peek_type(i));
				if (iter != token_registry.end())
				{
					j2 = std::find(expected_hierarchy[j].first.begin(), expected_hierarchy[j].first.end(), iter->first);
					if (j2 != expected_hierarchy[j].first.end())
						results.push_back(pair<int, int>(j, j2 - expected_hierarchy[j].first.begin()));
				}

				// Look for structures
				for (syntax_registry_iterator iter = syntax_registry.begin(); iter != syntax_registry.end(); iter++)
				{
					j2 = std::find(expected_hierarchy[j].first.begin(), expected_hierarchy[j].first.end(), iter->first);
					if (j2 != expected_hierarchy[j].first.end() && iter->second(config, *this, i))
						results.push_back(pair<int, int>(j, j2 - expected_hierarchy[j].first.begin()));
				}
			}
		}
	}

	if (results.size() > 1)
	{
		internal(*this, "ambiguous grammar", __FILE__, __LINE__, 1);
		for (int i = 0; i < (int)results.size(); i++)
			note(*this, ::to_string(results[i].first) + " " + ::to_string(results[i].second) + " " + ::to_string(expected_hierarchy[results[i].first].second) + " " + expected_hierarchy[results[i].first].first[results[i].second], __FILE__, __LINE__);
		return pair<int, int>(-1, -1);
	}
	else if (results.size() == 1)
		return results[0];

	return pair<int, int>(-1, -1);
}

pair<int, int> tokenizer::expected(string s)
{
	for (int i = (int)expected_hierarchy.size()-1; i >= 0; i--)
	{
		vector<string>::iterator i2 = std::find(expected_hierarchy[i].first.begin(), expected_hierarchy[i].first.end(), s);
		if (i2 != expected_hierarchy[i].first.end())
			return pair<int, int>(i, i2 - expected_hierarchy[i].first.begin());
	}
	return pair<int, int>(-1, -1);
}

int tokenizer::number_expected(int max_required_level)
{
	int total = 0;
	for (int i = (int)expected_hierarchy.size()-1; i >= max_required_level; i--)
		total += (int)expected_hierarchy[i].first.size();
	return total;
}

bool tokenizer::found(string s)
{
	return (found_type == s);
}

void tokenizer::insert(configuration &config, string name, string contents)
{
	segment_index++;

	segments.insert(segments.begin() + segment_index, segment());
	index.insert(index.begin() + segment_index, -1);

	// Parse the input to provide information about line numbers and tokens.
	segments[segment_index].name = name;
	segments[segment_index].buffer = contents;
	segments[segment_index].lines.push_back(0);

	offset = -1;
	while (offset < (int)segments[segment_index].buffer.size()-1)
	{
		char character = peek_char(1);
		if (character == '\r')
		{
			segments[segment_index].buffer.erase(offset+1, 1);
		}
		else if (character == '\n')
		{
			segments[segment_index].lines.push_back(offset+2);
			next_char();
		}
		else if (character == '\t' || character == ' ')
			next_char();
		else
		{
			vector<int> matching_comments;
			vector<token_registry_iterator> matching_tokens;
			for (int i = 0; i < (int)comment_registry.size(); i++)
				if (comment_registry[i].first(config, *this, 1))
					matching_comments.push_back(i);

			for (token_registry_iterator i = token_registry.begin(); i != token_registry.end(); i++)
				if (i->second.is_next(config, *this, 1))
					matching_tokens.push_back(i);

			if (matching_comments.size() + matching_tokens.size() > 1)
				internal(location(segment_index, offset+1), "ambiguous token set", __FILE__, __LINE__);

			if (matching_comments.size() > 0)
				comment_registry[matching_comments.back()].second(*this);
			else if (matching_tokens.size() > 0)
				segments[segment_index].tokens.push_back(matching_tokens.back()->second.consume(*this));
			else
				error(location(segment_index, offset+1), (string)"stray '" + character + "'", "", __FILE__, __LINE__);
		}
	}
}

bool tokenizer::is_next(string str, int i)
{
	return (peek(i) == str);
}

string tokenizer::file(int i)
{
	int temp_index = segment_index;
	int inc = (i >= 0 ? 1 : -1);
	while (temp_index < (int)segments.size() && temp_index >= 0 && (index[temp_index] + i >= (int)segments[temp_index].tokens.size() || index[temp_index]+i < 0))
	{
		i -= inc ? segments[temp_index].tokens.size() - index[temp_index] : -index[temp_index];
		temp_index += inc;
	}

	if (temp_index >= 0 && temp_index < (int)segments.size())
		return segments[temp_index].name;
	else
		return "";
}

string tokenizer::line(int i)
{
	int temp_index = segment_index;
	int inc = (i >= 0 ? 1 : -1);
	while (temp_index < (int)segments.size() && temp_index >= 0 && (index[temp_index] + i >= (int)segments[temp_index].tokens.size() || index[temp_index]+i < 0))
	{
		i -= inc ? segments[temp_index].tokens.size() - index[temp_index] : -index[temp_index];
		temp_index += inc;
	}

	if (temp_index >= 0 && temp_index < (int)segments.size())
		return segments[temp_index].get_line(segments[temp_index].token_to_line(index[temp_index]+i));
	else
		return "";
}

string tokenizer::location(int s, int o)
{
	string location = segments[s].name + ":" + ::to_string(segments[s].lines.size()) + ":";
	int l = 0, h = 0;
	for (int i = segments[s].lines.back(); i < o; i++)
	{
		l++;
		if (segments[s].buffer[i] == '\t')
			h += 8;
		else
			h++;
	}
	location += ::to_string(l) + "-" + ::to_string(h);
	return location;
}

int tokenizer::line_number(int i)
{
	int temp_index = segment_index;
	int inc = (i >= 0 ? 1 : -1);
	while (temp_index < (int)segments.size() && temp_index >= 0 && (index[temp_index] + i >= (int)segments[temp_index].tokens.size() || index[temp_index]+i < 0))
	{
		i -= inc ? segments[temp_index].tokens.size() - index[temp_index] : -index[temp_index];
		temp_index += inc;
	}

	if (temp_index >= 0 && temp_index < (int)segments.size())
		return segments[temp_index].token_to_line(index[temp_index]+i);
	else
		return -1;
}

int tokenizer::line_offset(int i)
{
	int temp_index = segment_index;
	int inc = (i >= 0 ? 1 : -1);
	while (temp_index < (int)segments.size() && temp_index >= 0 && (index[temp_index] + i >= (int)segments[temp_index].tokens.size() || index[temp_index]+i < 0))
	{
		i -= inc ? segments[temp_index].tokens.size() - index[temp_index] : -index[temp_index];
		temp_index += inc;
	}

	if (temp_index >= 0 && temp_index < (int)segments.size())
		return segments[temp_index].tokens[index[temp_index]+i].start - segments[temp_index].lines[segments[temp_index].token_to_line(index[temp_index]+i)];
	else
		return -1;
}

int tokenizer::segment_offset(int i)
{
	int temp_index = segment_index;
	int inc = (i >= 0 ? 1 : -1);
	while (temp_index < (int)segments.size() && temp_index >= 0 && (index[temp_index] + i >= (int)segments[temp_index].tokens.size() || index[temp_index]+i < 0))
	{
		i -= inc ? segments[temp_index].tokens.size() - index[temp_index] : -index[temp_index];
		temp_index += inc;
	}

	if (temp_index >= 0 && temp_index < (int)segments.size())
		return segments[temp_index].tokens[index[temp_index]+i].start;
	else
		return -1;
}

char tokenizer::curr_char()
{
	if (segment_index >= 0 && segment_index < (int)segments.size() && offset >= 0 && offset < (int)segments[segment_index].buffer.size())
		return segments[segment_index].buffer[offset];
	else
		return '\0';
}

char tokenizer::prev_char()
{
	offset--;

	if (segment_index >= 0 && segment_index < (int)segments.size() && offset >= 0 && offset < (int)segments[segment_index].buffer.size())
		return segments[segment_index].buffer[offset];
	else
		return '\0';
}

char tokenizer::next_char()
{
	offset++;

	if (segment_index >= 0 && segment_index < (int)segments.size() && offset >= 0 && offset < (int)segments[segment_index].buffer.size())
		return segments[segment_index].buffer[offset];
	else
		return '\0';
}

char tokenizer::peek_char(int i)
{
	int temp_offset = offset+i;
	if (segment_index >= 0 && segment_index < (int)segments.size() && temp_offset >= 0 && temp_offset < (int)segments[segment_index].buffer.size())
		return segments[segment_index].buffer[temp_offset];
	else
		return '\0';
}

string tokenizer::curr_type()
{
	if (segment_index >= 0 && segment_index < (int)segments.size() && index[segment_index] >= 0 && index[segment_index] < (int)segments[segment_index].tokens.size())
		for (token_registry_iterator i = token_registry.begin(); i != token_registry.end(); i++)
			if (i->second.type == segments[segment_index].tokens[index[segment_index]].type)
				return i->first;

	return "";
}

string tokenizer::curr()
{
	if (segment_index >= 0 && segment_index < (int)segments.size() && index[segment_index] >= 0 && index[segment_index] < (int)segments[segment_index].tokens.size())
		return segments[segment_index].get_token(index[segment_index]);
	else
		return "";
}

string tokenizer::prev()
{
	int remainder = 1;
	while (segment_index >= (int)segments.size() && index[segment_index] - remainder < 0)
	{
		remainder -= index[segment_index];
		index[segment_index] = 0;
		segment_index--;
	}

	if (segment_index < (int)segments.size())
	{
		index[segment_index] -= remainder;
		return segments[segment_index].get_token(index[segment_index]);
	}
	else
		return "";
}

string tokenizer::next()
{
	int remainder = 1;
	while (segment_index < (int)segments.size() && index[segment_index] + remainder >= (int)segments[segment_index].tokens.size())
	{
		remainder -= segments[segment_index].tokens.size() - index[segment_index];
		index[segment_index] = segments[segment_index].tokens.size();
		segment_index++;
	}

	if (segment_index < (int)segments.size())
	{
		index[segment_index] += remainder;
		return segments[segment_index].get_token(index[segment_index]);
	}
	else
		return "";
}

string tokenizer::peek_type(int i)
{
	int temp_index = segment_index;
	int inc = (i >= 0 ? 1 : -1);
	while (temp_index < (int)segments.size() && temp_index >= 0 && (index[temp_index] + i >= (int)segments[temp_index].tokens.size() || index[temp_index]+i < 0))
	{
		i -= inc ? segments[temp_index].tokens.size() - index[temp_index] : -index[temp_index];
		temp_index += inc;
	}

	if (temp_index >= 0 && temp_index < (int)segments.size())
		for (token_registry_iterator j = token_registry.begin(); j != token_registry.end(); j++)
			if (j->second.type == segments[temp_index].tokens[index[temp_index] + i].type)
				return j->first;

	return "";
}

string tokenizer::peek(int i)
{
	int temp_index = segment_index;
	int inc = (i >= 0 ? 1 : -1);
	while (temp_index < (int)segments.size() && temp_index >= 0 && (index[temp_index] + i >= (int)segments[temp_index].tokens.size() || index[temp_index]+i < 0))
	{
		i -= inc ? segments[temp_index].tokens.size() - index[temp_index] : -index[temp_index];
		temp_index += inc;
	}

	if (temp_index >= 0 && temp_index < (int)segments.size())
		return segments[temp_index].get_token(index[temp_index] + i);
	else
		return "";
}

bool tokenizer::segment_loaded(string name)
{
	for (int i = segment_index+1; i < (int)segments.size(); i++)
		if (segments[i].name == name)
			return true;

	return false;
}

bool tokenizer::segment_loading(string name)
{
	for (int i = 0; i <= segment_index; i++)
		if (segments[i].name == name)
			return true;

	return false;
}
