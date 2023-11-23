//№11 Дан целочисленный массив а, состоящий из n элементов. Разменять местами
//    максимальный и минимальный элементы массива.

#include <iostream>
#include <limits>
using namespace std;

int main() {
	const int n = 5;
	int a[n], i;
	for (i = 0; i < n; i++) {
		printf("a[%d] = ", i); 
		scanf_s("%d", &a[i]);
	}

	int min = INT_MAX, max = INT_MIN, p, q, t;
	for (i = 0; i < n; i++) {
		if (a[i] > max) {
			p = i;
			max = a[i];
		}
		if (a[i] < min) {
			q = i;
			min = a[i];
		}
	}

	t = a[q];
	a[q] = a[p];
	a[p] = t;

	printf("new sequence:\n");
	for (i = 0; i < n; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	return 0;
}