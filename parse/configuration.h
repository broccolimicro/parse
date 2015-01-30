/*
 * configuration.h
 *
 *  Created on: Oct 22, 2014
 *      Author: nbingham
 */

#include "common.h"
#include "segment.h"

#ifndef configuration_h
#define configuration_h

struct tokenizer;

struct configuration
{
	configuration();
	~configuration();

	map<string, void*> language_handles;
	vector<string> import_directories;
	vector<string> input_files;
	string working_directory;

	void load(tokenizer &tokens, string filename, string base = "");
	void add_import_directory(string directory);
	void set_working_directory(string filename);
	void load_language_handles(string directory);
	bool has_language(string language_name);
};

#endif
