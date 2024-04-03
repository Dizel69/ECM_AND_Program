//№8 Дано натуральное число n. Определить, является ли n простым числом.

#include <iostream>
using namespace std;

int main() {
	int n;
	scanf_s("%d", &n);

	int flag = 1;
	for (int i = 2; i <= n / 2; i++) {
		if (!(n % i)) {
			flag = 0;
			break;
		}
	}

	if (flag && n != 1) 
		printf("%d is a prime\n", n);
	else 
		printf("%d is not a prime\n", n);

	return 0;
}