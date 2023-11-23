//№13 Дано натуральное число n. Определить, какие цифры и сколько раз встречаются в
//    записи числа. (“Частотный анализатор”).

#include <iostream>
using namespace std;

int main() {
	int n;
	scanf_s("%d", &n);

	int x[10] = { 0 };
	while (n > 0) {
		x[n % 10]++;
		n /= 10;
	}

	for (int i = 0; i < 10; i++) {
		if (x[i]) {
			printf("digits \"%d\": %d\n", i, x[i]);
		}
	}

	return 0;
}