// №23 Дан целочисленный массив размерности n. Определить, сколько элементов массива
//     являются числами Фибоначчи, или выдать сообщение, если таких чисел нет.

#include <iostream>

int* create_arr(int n) {
	return new int[n];
}

void fill_arr(int* arr, int n) {
	printf("Enter array numbers: ");
	for (int i = 0; i < n; i++) {
		scanf_s("%d", arr++);
	}
}

void delete_arr(int* arr) {
	delete[] arr;
}

bool isfib(int n) {
	int n1 = 0, n2 = 1, n3 = 1;        // является ли число числом Фибоначчи

	while (n > n1) {
		n1 = n2;
		n2 = n3;
		n3 = n1 + n2;
	}

	return n == n1;
}

int count_fib(int* arr, int n) {      //подсчет чисел Фибоначчи
	int count = 0;
	for (int i = 0; i < n; i++) {
		if (isfib(arr[i])) count++;
	}

	return count;
}
int main() {
	int n; 
	printf("Enter the length of the array: ");
	scanf_s("%d", &n);
	int* a = create_arr(n);
	fill_arr(a, n);

	printf("%d fibonacci numbers\n", count_fib(a, n));

	delete_arr(a);

	return 0;
}