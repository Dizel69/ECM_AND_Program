//№12 Дана последовательность целых чисел х1, х2, …, хn. Определить, упорядочена ли эта
//    последовательность строго по возрастанию.


#include <iostream>
using namespace std;

int main() {
	const int n = 5;
	int a[n], i;
	
	for (i = 0; i < n; i++) {
		printf("a[%d] = ", i); 
		scanf_s("%d", &a[i]);
	}

	int flag = 1;
	for (i = 0; i < n - 1; i++) {
		if (a[i] >= a[i + 1]) {
			flag = 0;
			break;
		}
	}

	if (flag) 
		printf("Sequence is increasing\n");
	else 
		printf("Sequence is not increasing\n");

	return 0;
}
