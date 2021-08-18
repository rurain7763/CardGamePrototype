#pragma once

typedef void (*METHOD_FOR_DELETE)(void* parm);

struct Node
{
	Node* prev;
	void* data;
};

class iArray
{
public:
	iArray();
	iArray(METHOD_FOR_DELETE method);
	virtual ~iArray();

	void add(int idx,void* d);
	void push_back(void* d);
	void* at(int idx);
	void remove(int idx);
	void clear();

	void* operator[](int idx);

public:
	METHOD_FOR_DELETE m;
	Node* end;
	int num;
};