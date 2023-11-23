//№7 Дана последовательность целых чисел х1, х2, …, хn. Среди отрицательных элементов
//   последовательности найти минимальный элемент и его порядковый номер.


#include <iostream>
#include <limits>
using namespace std;

int main() {

	const int n = 5;
	int x[n];
	int max = INT_MIN;

	for (int i = 1; i < n; i++) {
		scanf_s("%d", &x[i]);
	}

	for (int i = 0; i <= n; i++)
		if (x[i] < 0 && x[i] > max) max = x[i];

	if (max < 0)
		printf("%d\n", max);
	else
		printf("net otricatelnih chisel\n");

}