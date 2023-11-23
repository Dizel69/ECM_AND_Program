//№16 Дан целочисленный массив а, состоящий из n элементов, и целое число М.
//    Найти элемент, который ближе всего к М. Выдать значение этого элемента и
//    его индекс. Предполагается, что такой элемент единственный.


#include <iostream>
#include <limits>
using namespace std;

int main() {
	int n, M, s, val, ind;
	int aboba = INT_MAX;
	int* a;

	printf("Enter the length of the array: \n");
	scanf_s("%i", &n);

	a = new int[n];

	printf("Enter array elents: \n");
	for (int i = 0; i < n; i++)
		scanf_s("%i", a + i);

	printf("Enter the number M: \n");
	scanf_s("%i", &M);

	for (int i = 0; i < n; i++) {
		s = abs(M - a[i]);

		if (s < aboba) {
			aboba = s;
			val = a[i];
			ind = i;
		}
	}

	printf("Closest to M: %i,\nIndex: %i", val, ind);

	return 0;
}