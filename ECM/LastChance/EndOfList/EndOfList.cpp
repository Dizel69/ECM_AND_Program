/* Написать программу, которая создает непустой линейный двухсвязный список, а затем в конец 
   этого списка добавляет все его элементы, располагая их в обратном порядке.  Исходные данные
   читаются из текстового файла, который содержит непустую последовательность слов, разделённых 
   одним и более пробелом. Длина слова не превышает восьми символов.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

struct Node {
	char inf[9];
	Node* prev;
	Node* next;
};

struct List {
	Node* head;
	Node* tail;
};

void DeleteList(List* list) {
	Node* temp = list->head;

	while (temp) {
		temp = temp->next;
		delete list->head;

		list->head = temp;
	}
}

Node* NodeInit() {
	Node* node = new Node;
	node->next = nullptr;
	node->prev = nullptr;
	return node;
}

List* ListInit() {
	List* list = new List;

	list->head = NodeInit();
	list->tail = list->head;

	return list;
}

void ListPrint(List* list) {
	Node* temp = list->head;
	while (temp) {
		printf("%s ", temp->inf);
		temp = temp->next;
	}
}

void AddElement(List*& list, char* str) {
	if (!list) {
		list = ListInit();
		strcpy(list->head->inf, str);
		list->tail = list->head;
	}
	else {
		Node* temp = list->tail;
		Node* next = temp->next;

		next = NodeInit();
		temp->next = next;
		next->prev = temp;
		temp = next;

		strcpy(temp->inf, str);
		list->tail = temp;
	}
}

List* CreateListFromFile(const char* filename) {
	FILE* f = fopen(filename, "r");
	if (!f) {
		printf("error");
		exit(1);
	}

	List* list = NULL;
	char str[9];
	while (fscanf(f, "%s", str) == 1) {
		AddElement(list, str);
	}

	fclose(f);
	return list;
}

List* AddWordsFromEnd(List* list) {
	Node* newOne = list->tail->next, * oldOne = list->tail;
	while (oldOne) {
		AddElement(list, oldOne->inf);

		oldOne = oldOne->prev;
	}

	return list;
}

int main() {
	List* list = CreateListFromFile("bubas.txt");

	ListPrint(list); printf("\n");

	list = AddWordsFromEnd(list);

	ListPrint(list);
	DeleteList(list);
}
