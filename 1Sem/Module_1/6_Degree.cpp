//№6 Дано натуральное число n. Определить, является ли n степенью числа 2.

#include <iostream>
using namespace std;

int main() {
	int n;
	scanf_s("%d", &n);

	int flag = 0, p = 1;
	for (int i = 1; i < 31; i++) {
		if (n == p) {
			flag = 1;
			break;
		}
		p <<= 1;
	}


	if (flag) printf("%d is a power of 2\n", n);
	else printf("%d is not a power of 2\n", n);

	return 0;
}