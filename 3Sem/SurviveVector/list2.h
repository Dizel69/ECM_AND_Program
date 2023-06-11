#pragma once
#include <string>
#include <iostream>

struct obj
{
	std::string name;
	obj* next;
	obj* prev;
};

class list2
{
	obj* head;
	int count;
public:
	list2(int c = 0);
	void fill();
	void print();
	void game(int k);
};

