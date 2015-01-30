/*
 * message.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: nbingham
 */

#include "message.h"
#include "tokenizer.h"

int num_errors = 0;
int num_warnings = 0;
int num_internal = 0;
int num_notes = 0;
int num_log = 0;
bool verbose = true;
bool debug = true;

void internal(tokenizer &tokens, string internal, string debug_file, int debug_line, int offset)
{
	int number = tokens.line_number();

	int column_start = tokens.line_offset() + 1 + offset;
	int column_end = 1;
	string line = tokens.line();
	string str;
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

	cout << debug_file << ":" << debug_line << ":";
	cout << tokens.file() << ":" << number+1 << ":" << column_start << "-" << column_end << ": internal failure: " << internal << endl;
	cout << line << endl;
	cout << str << "^" << endl;

	num_internal++;
}

void error(tokenizer &tokens, string error, string note, string debug_file, int debug_line, int offset)
{
	int number = tokens.line_number();

	int column_start = tokens.line_offset() + 1 + offset;
	int column_end = 1;
	string line = tokens.line();
	string str;
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

	if (debug)
		cout << debug_file << ":" << debug_line << ":";

	cout << tokens.file() << ":" << number+1 << ":" << column_start << "-" << column_end << ": error: " << error << endl;
	if (note != "")
	{
		if (debug)
			cout << debug_file << ":" << debug_line << ":";

		cout << tokens.file() << ":" << number+1 << ":" << column_start << "-" << column_end << ": note: " << note << endl;
	}
	cout << line << endl;
	cout << str << "^" << endl;

	num_errors++;
}

void warning(tokenizer &tokens, string warning, string note, string debug_file, int debug_line, int offset)
{
	int number = tokens.line_number();

	int column_start = tokens.line_offset() + 1 + offset;
	int column_end = 1;
	string line = tokens.line();
	string str;
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

	if (debug)
		cout << debug_file << ":" << debug_line << ":";

	cout << tokens.file() << ":" << number+1 << ":" << column_start << "-" << column_end << ": warning: " << warning << endl;
	if (note != "")
	{
		if (debug)
			cout << debug_file << ":" << debug_line << ":";

		cout << tokens.file() << ":" << number+1 << ":" << column_start << "-" << column_end << ": note: " << note << endl;
	}
	cout << line << endl;
	cout << str << "^" << endl;

	num_warnings++;
}

void note(tokenizer &tokens, string note, string debug_file, int debug_line, int offset)
{
	int number = tokens.line_number();

	int column_start = tokens.line_offset() + 1 + offset;
	int column_end = 1;
	string line = tokens.line();
	string str;
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

	if (debug)
		cout << debug_file << ":" << debug_line << ":";

	cout << tokens.file() << ":" << number+1 << ":" << column_start << "-" << column_end << ": note: " << note << endl;
	cout << line << endl;
	cout << str << "^" << endl;

	num_notes++;
}

void log(tokenizer &tokens, string log, string debug_file, int debug_line, int offset)
{
	if (verbose)
	{
		if (debug)
			cout << debug_file << ":" << debug_line << ":";

		if (tokens.segment_index >= 0 && tokens.segment_index < (int)tokens.segments.size() && tokens.index[tokens.segment_index] >= 0 && tokens.index[tokens.segment_index] < (int)tokens.segments[tokens.segment_index].buffer.size())
			cout << tokens.file() << ":" << tokens.line_number()+1 << ":";

		cout << "log:\t" << log << endl;

		num_log++;
	}
}

void internal(string location, string internal, string debug_file, int debug_line)
{
	cout << debug_file << ":" << debug_line << ":";
	if (location != "")
		cout << location << ": ";
	cout << "internal failure: " << internal << endl;
	num_internal++;
}

void error(string location, string error, string note, string debug_file, int debug_line)
{
	if (debug)
		cout << debug_file << ":" << debug_line << ":";

	if (location != "")
		cout << location << ": ";
	cout << "error: " << error << endl;
	if (note != "")
	{
		if (debug)
			cout << debug_file << ":" << debug_line << ":";

		if (location != "")
			cout << location << ": ";
		cout << "note: " << note << endl;
	}

	num_errors++;
}

void warning(string location, string warning, string note, string debug_file, int debug_line)
{
	if (debug)
		cout << debug_file << ":" << debug_line << ":";

	if (location != "")
		cout << location << ": ";
	cout << "warning: " << warning << endl;
	if (note != "")
	{
		if (debug)
			cout << debug_file << ":" << debug_line << ":";

		if (location != "")
			cout << location << ": ";
		cout << "note: " << note << endl;
	}

	num_warnings++;
}

void note(string location, string note, string debug_file, int debug_line)
{
	if (debug)
		cout << debug_file << ":" << debug_line << ":";

	if (location != "")
		cout << location << ": ";
	cout << "note: " << note << endl;

	num_notes++;
}

void log(string location, string log, string debug_file, int debug_line)
{
	if (verbose)
	{
		if (debug)
		{
			cout << debug_file << ":" << debug_line << ":";
			if (location == "")
				cout << "\t";
		}

		if (location != "")
			cout << location << ":\t";
		cout << log << endl;

		num_log++;
	}
}

void progress(string location, string log, string debug_file, int debug_line)
{
	cout << "\r";
	if (debug)
	{
		cout << debug_file << ":" << debug_line << ":";
		if (location == "")
			cout << "\t";
	}
	if (location != "")
		cout << location << ":\t";
	cout << log << "                    ";
	cout.flush();
}

void done_progress()
{
	cout << "\r";
	cout.flush();
}

void complete()
{
	cout << "completed with ";
	if (num_internal > 0)
		cout << num_internal << " internal failures, ";
	cout << num_errors << " errors and " << num_warnings << " warnings" << endl;
}

bool is_clean()
{
	return (num_internal == 0 && num_errors == 0);
}

void set_debug(bool value)
{
	debug = value;
}

void set_verbose(bool value)
{
	verbose = value;
}
