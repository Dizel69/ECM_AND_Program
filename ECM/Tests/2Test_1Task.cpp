//2 контрольная 1 задание
//  Программа проверяет является ли число факториалом какого либо числа

#include <iostream>

int Fact_Cheker(int n) {
	int flag = 0;
	int fact = 1;
	
	for (int i = 2; i < n; i++) {
		fact = fact * i;
		if (fact == n)
			flag = 1;
	}
	
	return flag;
}

int main() {
	int n;
	printf("Enter the n you want to check: ");
	scanf_s("%d", &n);

	int k, flag = 0;

	k = Fact_Cheker(n);

	if (k == 1)
		printf("This number is factorial");
	else
		printf("This number is not a factorial");

	return 0;
}