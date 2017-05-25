/*
 * segment.h
 *
 *  Created on: Oct 15, 2014
 *      Author: nbingham
 */

#include <std/string.h>

#pragma once

using namespace core;

struct segment
{
	struct iterator
	{
		segment *root;
		array<string>::iterator line;
		string::iterator c;

		iterator();
		iterator(segment *root, array<string>::iterator line, string::iterator c);
		~iterator();

		operator bool() const;

		char &operator*() const;
		char *ptr() const;
		char &get() const;
		
		iterator &operator++(int);
		iterator &operator--(int);
		iterator &operator++();
		iterator &operator--();
		iterator &operator+=(int n);
		iterator &operator-=(int n);
		iterator operator+(int n) const;
		iterator operator-(int n) const;
		bool operator==(iterator i) const;
		bool operator!=(iterator i) const;
		bool operator<(iterator i) const;
		bool operator>(iterator i) const;
		bool operator<=(iterator i) const;
		bool operator>=(iterator i) const;
		int operator-(iterator i) const;
		
		string file_name();
		int line_number();
		int char_number();
		pair<int, int> col_number();

		string report();
		string pointer(int length = 1);

		void note(string msg, int length = 1);
		void warn(string msg, int length = 1);
		void error(string msg, int length = 1);
	};

	segment();
	~segment();

	string name;
	array<string> lines;

	iterator begin();
	iterator end();
	iterator rbegin();
	iterator rend();

	void load(string name);
};


