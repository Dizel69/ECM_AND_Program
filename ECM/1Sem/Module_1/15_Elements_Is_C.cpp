//№15 Дан целочисленный массив а, состоящий из n элементов, и целое число С.
//    Если в массиве есть элементы равные С, то вывести номер первого такого
//    элемента, если таких элементов нет, то выдать сообщение. («Линейный поиск»).

#include <iostream>
using namespace std;

int main() {
	const int n = 5;
	int a[n], i;
	
	for (i = 0; i < n; i++) {
		printf("a[%d] = ", i); 
		scanf_s("%d", &a[i]);
	}

	int C, ind = -1;
	
	printf("number to search: "); 
	scanf_s("%d", &C);

	for (i = 0; i < n; i++) {
		if (a[i] == C) {
			ind = i;
			break;
		}
	}

	if (ind >= 0) 
		printf("element %d has index %d\n", C, ind);
	else 
		printf("no %d in sequence\n", C);

	return 0;
}