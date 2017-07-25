#include <std/file.h>
#include <std/fill.h>
#include "segment.h"

using namespace core;

segment::segment()
{
}

segment::segment(string name)
{
	load(name);
}

segment::~segment()
{
}

void segment::load(string name)
{
	this->name = name;
	file buf(name.c_str(), "r");

	while (buf)
	{
		string line = buf.read("\n");
		if (line.size() > 0)
			lines.push_back(line);
	}
}

segment::iterator segment::begin()
{
	return segment::iterator(this, lines.begin(), lines.begin()->begin());
}

segment::iterator segment::end()
{
	return segment::iterator(this, lines.end(), string::iterator());
}

segment::iterator segment::rbegin()
{
	return segment::iterator(this, lines.rbegin(), lines.rbegin()->rbegin());
}

segment::iterator segment::rend()
{
	return segment::iterator(this, lines.rend(), string::iterator());
}




// Segment Iterator

segment::iterator::iterator()
{
	root = NULL;
}

segment::iterator::iterator(segment *root, array<string>::iterator line, string::iterator c)
{
	this->root = root;
	this->line = line;
	this->c = c;
}

segment::iterator::~iterator()
{
}

segment::iterator::operator bool() const
{
	return root != NULL && line && c;
}
		
char &segment::iterator::operator*() const
{
	return *c;
}

char *segment::iterator::ptr() const
{
	return c.ptr();
}

char &segment::iterator::get() const
{
	return c.get();
}
		
segment::iterator &segment::iterator::operator++(int)
{
	if (line)
	{
		c++;
		while (!c && line)
		{
			line++;
			if (line)
				c = line->begin();
			else
				c = string::iterator();
		}
	}

	return *this;
}

segment::iterator &segment::iterator::operator--(int)
{
	if (line)
	{
		c--;
		while (!c && line)
		{
			line--;
			if (line)
				c = line->rbegin();
			else
				c = string::iterator();
		}
	}

	return *this;
}

segment::iterator &segment::iterator::operator++()
{
	if (line)
	{
		c++;
		while (!c && line)
		{
			line++;
			if (line)
				c = line->begin();
			else
				c = string::iterator();
		}
	}

	return *this;
}

segment::iterator &segment::iterator::operator--()
{
	if (line)
	{
		c--;
		while (!c && line)
		{
			line--;
			if (line)
				c = line->rbegin();
			else
				c = string::iterator();
		}
	}

	return *this;
}

segment::iterator &segment::iterator::operator+=(int n)
{
	while (line && n >= (line->end() - c))
	{
		n -= (line->end() - c);
		line++;
		if (line)
			c = line->begin();
		else
			c = string::iterator();
	}

	if (c)
		c += n;

	return *this;
}

segment::iterator &segment::iterator::operator-=(int n)
{
	while (line && n > (c - line->begin()))
	{
		n -= (c - line->begin());
		line--;
		if (line)
			c = line->rbegin();
		else
			c = string::iterator();
	}

	if (c)
		c -= n;

	return *this;
}

segment::iterator segment::iterator::operator+(int n) const
{
	segment::iterator result = *this;
	result += n;
	return result;
}

segment::iterator segment::iterator::operator-(int n) const
{
	segment::iterator result = *this;
	result -= n;
	return result;
}

bool segment::iterator::operator==(iterator i) const
{
	return line == i.line && c == i.c;
}

bool segment::iterator::operator!=(iterator i) const
{
	return line != i.line || c != i.c;
}

bool segment::iterator::operator<(iterator i) const
{
	return line < i.line || (line == i.line && c < i.c);
}

bool segment::iterator::operator>(iterator i) const
{
	return line > i.line || (line == i.line && c > i.c);
}

bool segment::iterator::operator<=(iterator i) const
{
	return line < i.line || (line == i.line && c <= i.c);
}

bool segment::iterator::operator>=(iterator i) const
{
	return line > i.line || (line == i.line && c >= i.c);
}

int segment::iterator::operator-(iterator i) const
{
	int result = 0;
	while (i && i.line < line)
	{
		result += i.line->end() - i.c;
		i.line++;
		i.c = i.line->begin();
	}
	result += c - i.c;
	return result;
}

string segment::iterator::file_name()
{
	return root->name;
}

int segment::iterator::line_number()
{
	return line.idx();
}

int segment::iterator::char_number()
{
	return c.idx();
}

pair<int, int> segment::iterator::col_number()
{
	pair<int, int> result(0, 0);
	for (string::iterator i = line->begin(); i != c; i++)
	{
		if (*i == '\t')
		{
			result.first += 1;
			result.second += 4;
		}
		else
		{
			result.first += 1;
			result.second += 1;
		}
	}
	return result;
}

string segment::iterator::report()
{
	pair<int, int> col = col_number();
	string result;
	result << file_name() << ":" << line_number();
	result << ":" << col.first << "-" << col.second;
	return result;
}

string segment::iterator::pointer(int length)
{
	string result = *line;
	string::iterator i;
	for (i = result.begin(); i != c; i++)
		if (*i != '\t')
			*i = ' ';
	i.drop(result.size());
	result << string(fill<char>(max(length, 1), '^')) << "\n";
	return result;
}

