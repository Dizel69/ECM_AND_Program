//№14 Дана последовательность целых чисел а1, а2, …, аn. Осуществить циклический сдвиг
//    элементов последовательности
//       -на одну позицию вправо;
//       -на к позиций вправо.

#include <iostream>
using namespace std;

int main() {
	const int n = 10;
	int a[n], i;
	for (i = 0; i < n; i++) {
		printf("a[%d] = ", i); scanf_s("%d", &a[i]);
	}

	int k, t;
	printf("number of moves: "); scanf_s("%d", &k);
	for (i = 0; i < k % n; i++) {
		t = a[n - 1];
		for (int j = n - 1; j > 0; j--) {
			a[j] = a[j - 1];
		}
		a[0] = t;
	}

	printf("the result of %d moves:\n", k);
	for (i = 0; i < n; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	return 0;
}