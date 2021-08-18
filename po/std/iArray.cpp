#include"iArray.h"

iArray::iArray()
{
	m = nullptr;
	end = nullptr;
	num = 0;
}

iArray::iArray(METHOD_FOR_DELETE method)
	:iArray()
{
	m = method;
}

iArray::~iArray()
{
	Node* t = end;

	while(t)
	{
		Node* c = t->prev;
		if (m) m(t->data);
		delete t;
		t = c;
	}
}

void iArray::add(int idx, void* d)
{
	Node* t = end;

	for (int i = num - 1; i != idx; i--)
	{
		t = t->prev;
	}

	Node* n = new Node{ nullptr,d };
	n->prev = t->prev;
	t->prev = n;
	num++;
}

void iArray::push_back(void* d)
{
	Node* n = new Node{ nullptr,d };
	n->prev = end;
	end = n;
	num++;
}

void* iArray::at(int idx)
{
	Node* t = end;

	for (int i = num - 1; i != idx; i--)
	{
		t = t->prev;
	}

	return t->data;
}

void iArray::remove(int idx)
{
	if (idx == num - 1)
	{
		Node* c = end->prev;
		if (m) m(end->data);
		delete end;
		end = c;
		num--;
	}
	else
	{
		Node* t = end;

		for (int i = num - 1; i != idx+1; i--)
		{
			t = t->prev;
		}

		Node* c = t->prev->prev;
		if (m) m(t->data);
		delete t->prev;
		t->prev = c;
		num--;
	}
}

void iArray::clear()
{
	Node* t = end;

	while(t)
	{
		Node* next = t->prev;
		if (m) m(t->data);
		delete t;
		t = next;
	}

	end = nullptr;
	num = 0;
}

void* iArray::operator[](int idx)
{
	Node* t = end;

	for (int i = num - 1; i != idx; i--)
	{
		t = t->prev;
	}

	return t->data;
}
