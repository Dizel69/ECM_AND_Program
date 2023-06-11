#include "list2.h"

list2::list2(int c) {
	count = c;
	if (c == 0)
	{
		head = nullptr;
	}
	else
	{
		head = new obj;
		head->name = "0";
		obj* cur = head, * t;
		for (int i = 1; i < c; i++)
		{
			t = new obj;
			t->name = i;
			cur->next = t;
			t->prev = cur;
			cur = cur->next;
		}
		cur->next = head;
		head->prev = cur;
	}
}

void list2::fill() {
	std::cout << "Заполните участников" << std::endl;
	obj* cur = head;
	for (int i = 0; i < count; i++)
	{
		std::cin >> cur->name;
		cur = cur->next;
	}
}

void list2::print() {
	obj* cur = head;
	for (int i = 0; i < count; i++)
	{
		std::cout << cur->name << std::endl;
		cur = cur->next;
	}
}

void list2::game(int k) {
	obj* cur = head, * cur1;
	while (count > 1)
	{
		for (int i = 0; i < k - 1; i++)
		{
			cur = cur->next;
		}
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		cur1 = cur;
		std::cout << cur->name << std::endl;
		cur = cur->next;
		delete cur1;
		count--;
	}
	head = cur;
}