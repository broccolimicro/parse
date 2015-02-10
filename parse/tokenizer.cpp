/*
 * tokenizer.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: nbingham
 */

#include "tokenizer.h"

tokenizer::tokenizer()
{
	segment_index = -1;
	offset = -1;
}

tokenizer::~tokenizer()
{

}

void tokenizer::internal(string internal, string debug_file, int debug_line, int offset)
{
	int number = line_number(offset);
	int column_start = line_offset(offset);
	string line = this->line(offset);
	string file = this->file(offset);

	string str;
	int column_end = 1;
	for (int j = 0; j < (int)line.size() && j < column_start; j++)
	{
		if (line[j] != '\t')
		{
			str.push_back(' ');
			column_end++;
		}
		else
		{
			str.push_back('\t');
			column_end+=8;
		}
	}

	string location = file + ":" + ::to_string(number+1) + ":" + ::to_string(column_start) + "-" + ::to_string(column_end);

	::internal(location, internal, debug_file, debug_line);
	cout << line << endl;
	cout << str << "^" << endl;
}

void tokenizer::error(string error, string debug_file, int debug_line, int offset)
{
	int number = line_number(offset);
	int column_start = line_offset(offset);
	string line = this->line(offset);
	string file = this->file(offset);

	string str;
	int column_end = 1;
	for (int j = 0; j < (int)line.size() && j < column_start; j++)
	{
		if (line[j] != '\t')
		{
			str.push_back(' ');
			column_end++;
		}
		else
		{
			str.push_back('\t');
			column_end+=8;
		}
	}

	string location = file + ":" + ::to_string(number+1) + ":" + ::to_string(column_start) + "-" + ::to_string(column_end);
	::error(location, error, debug_file, debug_line);
	cout << line << endl;
	cout << str << "^" << endl;
}

void tokenizer::warning(string warning, string debug_file, int debug_line, int offset)
{
	int number = line_number(offset);
	int column_start = line_offset(offset);
	string line = this->line(offset);
	string file = this->file(offset);

	string str;
	int column_end = 1;
	for (int j = 0; j < (int)line.size() && j < column_start; j++)
	{
		if (line[j] != '\t')
		{
			str.push_back(' ');
			column_end++;
		}
		else
		{
			str.push_back('\t');
			column_end+=8;
		}
	}

	string location = file + ":" + ::to_string(number+1) + ":" + ::to_string(column_start) + "-" + ::to_string(column_end);
	::warning(location, warning, debug_file, debug_line);
	cout << line << endl;
	cout << str << "^" << endl;
}

void tokenizer::note(string note, string debug_file, int debug_line, int offset)
{
	int number = line_number(offset);
	int column_start = line_offset(offset);
	string line = this->line(offset);
	string file = this->file(offset);

	string str;
	int column_end = 1;
	for (int j = 0; j < (int)line.size() && j < column_start; j++)
	{
		if (line[j] != '\t')
		{
			str.push_back(' ');
			column_end++;
		}
		else
		{
			str.push_back('\t');
			column_end+=8;
		}
	}

	string location = file + ":" + ::to_string(number+1) + ":" + ::to_string(column_start) + "-" + ::to_string(column_end);
	::note(location, note, debug_file, debug_line);
	cout << line << endl;
	cout << str << "^" << endl;
}

void tokenizer::log(string log, string debug_file, int debug_line, int offset)
{
	int number = line_number(offset);
	int column_start = line_offset(offset);
	string line = this->line(offset);
	string file = this->file(offset);

	string str;
	int column_end = 1;
	for (int j = 0; j < (int)line.size() && j < column_start; j++)
	{
		if (line[j] != '\t')
		{
			str.push_back(' ');
			column_end++;
		}
		else
		{
			str.push_back('\t');
			column_end+=8;
		}
	}

	string location = file + ":" + ::to_string(number+1) + ":" + ::to_string(column_start) + "-" + ::to_string(column_end);
	::log(location, log, debug_file, debug_line);
}

void tokenizer::syntax_start(parse::syntax *syntax)
{
	syntax->segment_name = segments[segment_index].name;
	syntax->start = index[segment_index]+1;
}

void tokenizer::syntax_end(parse::syntax *syntax)
{
	if (syntax->start != -1)
	{
		syntax->end = index[segment_index];
		syntax->valid = true;
	}
}

bool tokenizer::save(string key, const parse::syntax *syntax)
{
	if (syntax == NULL || !syntax->valid)
		return false;
	else
		return bookmarks.insert(pair<string, pair<string, int> >(key, pair<string, int>(syntax->segment_name, syntax->start))).second;
}

bool tokenizer::load(const parse::syntax *syntax)
{
	if (syntax == NULL || !syntax->valid)
		return false;

	bool found = false;
	for (int i = 0; i < (int)segments.size() && !found; i++)
		if (segments[i].name == syntax->segment_name)
		{
			segment_index = i;
			found = true;
		}

	if (found)
	{
		index[segment_index] = syntax->start;
		return true;
	}
	else
		return false;
}

bool tokenizer::load(string key)
{
	map<string, pair<string, int> >::iterator iter = bookmarks.find(key);
	if (iter != bookmarks.end())
	{
		bool found = false;
		for (int i = 0; i < (int)segments.size() && !found; i++)
			if (segments[i].name == iter->second.first)
			{
				segment_index = i;
				found = true;
			}

		if (found)
		{
			index[segment_index] = iter->second.second;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool tokenizer::erase(string key)
{
	map<string, pair<string, int> >::iterator iter = bookmarks.find(key);
	if (iter != bookmarks.end())
	{
		bookmarks.erase(iter);
		return true;
	}
	else
		return false;
}

tokenizer::level tokenizer::increment(bool required)
{
	expected_hierarchy.push_back(pair<vector<string>, bool>(vector<string>(), required));
	return (expected_hierarchy.end()-1);
}

tokenizer::level tokenizer::increment(level it, bool required)
{
	return expected_hierarchy.insert(it, pair<vector<string>, bool>(vector<string>(), required));
}

bool tokenizer::decrement(string debug_file, int debug_line, void *data)
{
	pair<int, int> idx(-1, -1);

	int max_required_level;
	for (max_required_level = (int)expected_hierarchy.size()-1; max_required_level >= 0 && !expected_hierarchy[max_required_level].second; max_required_level--);

	idx = expected(1, data);
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

		error(error_string, debug_file, debug_line, 1);

		while ((idx = expected(1, data)).first < max_required_level && next() != "");
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

void tokenizer::expect(level it, string s)
{
	it->first.push_back(s);
}

void tokenizer::expect(level it, vector<string> &s)
{
	it->first.insert(it->first.end(), s.begin(), s.end());
}

pair<int, int> tokenizer::expected(int i, void *data)
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
					if (j2 != expected_hierarchy[j].first.end() && iter->second(*this, i, data))
						results.push_back(pair<int, int>(j, j2 - expected_hierarchy[j].first.begin()));
				}
			}
		}
	}

	if (results.size() > 1)
	{
		internal("ambiguous grammar", __FILE__, __LINE__, 1);
		for (int i = 0; i < (int)results.size(); i++)
			::note("", ::to_string(results[i].first) + " " + ::to_string(results[i].second) + " " + ::to_string(expected_hierarchy[results[i].first].second) + " " + expected_hierarchy[results[i].first].first[results[i].second], __FILE__, __LINE__);
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

void tokenizer::insert(string name, string contents, void *data)
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
			segments[segment_index].buffer.erase(offset+1, 1);
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
				if (comment_registry[i].first(*this, 1, data))
					matching_comments.push_back(i);

			for (token_registry_iterator i = token_registry.begin(); i != token_registry.end(); i++)
				if (i->second.is_next(*this, 1, data))
					matching_tokens.push_back(i);

			if (matching_comments.size() + matching_tokens.size() > 1)
				::internal(location(segment_index, offset+1), "ambiguous token set", __FILE__, __LINE__);

			if (matching_comments.size() > 0)
				comment_registry[matching_comments.back()].second(*this, data);
			else if (matching_tokens.size() > 0)
				segments[segment_index].tokens.push_back(matching_tokens.back()->second.consume(*this, data));
			else
				::error(location(segment_index, offset+1), (string)"stray '" + character + "'", __FILE__, __LINE__);
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
