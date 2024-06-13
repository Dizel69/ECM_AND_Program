//№10 Дана последовательность целых чисел а1, а2, …, аn. Удалить, если есть, элементы
//    равные 0. Проверить, является ли, полученная последовательность
//    знакочередующейся.

#include <iostream>
using namespace std;

int main() {
	const int n = 5;
	int a[n], i, sgn1, sgn2, j = 0, flag = 1;
	
	for (i = 0; i < n; i++) {
		printf("a[%d] = ", i); 
		scanf_s("%d", &a[i]);
		
		if (a[i]) {
			a[j] = a[i];
			j++;
		}
	}

	if (a[0] > 0) sgn1 = 1;
	else sgn1 = -1;
	for (i = 1; i < j; i++) {
		if (a[i] > 0) sgn2 = 1;
		else sgn2 = -1;

		if (sgn1 == sgn2) {
			flag = 0;
			break;
		}

		sgn1 = sgn2;
	}

	printf("sequence without 0:\n");
	for (i = 0; i < j; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	if (j <= 1) printf("can't check alternativity\n");
	else if (flag) printf("the sequence is alternating\n");
	else printf("the sequence is not alternating\n");

	return 0;
}