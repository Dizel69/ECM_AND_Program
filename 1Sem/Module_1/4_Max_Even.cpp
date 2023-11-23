//№4 Дана последовательность целых чисел х 1 , х 2 , … х n . Определить, является ли
//   максимальный элемент последовательности четным числом.

#include <iostream>

using namespace std;

int main() {
	int n;
	
	printf("amount: "); 
	scanf_s("%d", &n);

	int x, max = INT_MIN;
	
	for (int i = 0; i < n; i++) {
		printf("element: "); 
		scanf_s("%d", &x);
		
		if (x > max) max = x;
	}

	if (max % 2) 
		printf("the largest element %d is odd\n", max);
	else 
		printf("the largest element %d is even\n", max);

	return 0;
}