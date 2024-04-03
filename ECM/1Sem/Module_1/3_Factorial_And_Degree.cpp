//№3 Дано натуральное число n. Если n < 10, то положить М=n!, иначе положить М=n k ,
//   где k подается с ввода.

#include <iostream>
using namespace std;

int main() {
	
	int n;
	scanf_s("%d", &n);

	if (n < 10) {
		int mult = 1, fact = 1;
		
		while (mult <= n) {
			fact *= mult++;
		}
		printf("%d! = %d\n", n, fact);
	}
	else {
		
		int k;
		
		scanf_s("%d", &k);

		
		int prod = 1;
		for (int i = 0; i < k; i++) {
			prod *= n;
		}

		printf("%d^%d = %d\n", n, k, prod);
	}

	return 0;
}