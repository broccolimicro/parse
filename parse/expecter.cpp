#include "expecter.h"

expecter::expecter(bool required)
{
	this->required = required;
}

expecter::~expecter()
{
}

void expecter::expect(parse_fn p, const void *data)
{
	options.push_back(pair<parse_fn, const void*>(p, data));
}

