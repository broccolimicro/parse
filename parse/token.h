#pragma once

struct token
{
	token();
	token(int s, int e, int t);
	~token();

	int start;
	int end;
	int type;
};

bool operator<(token t1, token t2);

