//№5 Дана последовательность целых чисел а1, а2, …, аn. Найти сумму элементов
//   последовательности, которые оканчиваются цифрой 5, или выдать сообщение, что
//   таких элементов нет.

#include <iostream>
using namespace std;

int main() {
	int n;
	
	printf("amount: "); 
	scanf_s("%d", &n);

	int a, g = 0;
	int sum = 0;
	
	for (int i = 0; i < n; i++) {
		printf("element: "); 
		scanf_s("%d", &a);
		
		if (a % 10 == 5) {
			sum += a;
			g += 1;
		}
		
	}
	
	if (g != 0)
		printf("Sum of elements with the last digit 5: %d\n", sum);
	else
		printf("There are no numbers ending in 5");

	return 0;

}
