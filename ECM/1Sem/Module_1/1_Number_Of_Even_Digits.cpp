//№1 Дано число n, определите количство чётных цифр в числе

#include <iostream>
using namespace std;

int main() {
	
	int n, m, ost, count;

	printf("Vvedite chislo n \n");
	scanf_s("%d", &n);

	m = n; count = 0;

	while (m > 0) {
		ost = m % 10;
		
		if (ost % 2 == 0)
			count = count + 1;
		
		m = m / 10;
	}
	
	if (count > 0)
		printf("%d \n", count);
	else
		printf("No \n");

	return 0;
	
}