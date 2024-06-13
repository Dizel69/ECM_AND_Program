//1 Вариант задание 18
//        Найти все  простые числа, не превосходящие заданного N, в двоичной записи,  которых  единиц  больше чем нулей.


#include <iostream>

int isPrime(int h) {      //Проверка числа на простоту 
	int r = 0;

	int flag = 1;
	for (int i = 2; i <= h / 2; i++) {
		if (!(h % i)) {
			flag = 0;
			break;
		}
	}

	if (flag && h != 1)
		r = 1;
	else
		r = 0;

	return r;
}

int getBinaryInt(int h) {     // Переводим число в двоичную систему счисления
	int bin = 0;
	int rem, i = 1;
	while (h != 0) {
		rem = h % 2;
		h /= 2;
		bin += rem * i;
		i *= 10;
	}
	return bin;
}

int main() {
	
	int N, one = 0, zero = 0, r = 0, h = 2;
	int bin = 0;
		
	printf("Enter the number N:\n");
	scanf_s("%d", &N);

	
	while (h <= N) {
		r = isPrime(h);
		bin = getBinaryInt(h);
		if (r == 1) {
			while (bin > 0) {
				if (bin % 10 == 0) {
					zero++;
				}
				else
					one++;
				bin = bin / 10;
			}
		
			if (zero < one)
				printf("%d, ", h);
		}

		one = 0;
		zero = 0;
		h++;
	}
	return 0;
}