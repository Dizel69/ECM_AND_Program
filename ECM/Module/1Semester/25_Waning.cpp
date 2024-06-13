//№25 Дан целочисленный массив А размерности n и целочисленный массив В
//    размерности m.Положить k равным 2, если оба массива упорядочены по не
//    убыванию, равным 1, если один из массивов упорядочен по не убыванию, и
//    равным 0, если оба массива не упорядочены по не убыванию.


#include <iostream>

int* create_arr(int n) {
	return new int[n];
}

void fill_arr(int* arr, int n) {
	for (int i = 0; i < n; i++) {
		scanf_s("%d", arr++);
	}
}

void delete_arr(int* arr) {
	delete[] arr;
}

int isnondecr(int* arr, int n) {  // проверка на неубывание
	for (int i = 0; i < n - 1; i++) {
		if (arr[i] > arr[i + 1]) return 0;
	}
	return 1;
}

int main() {
	int n; 
	printf("Enter the length of the array A: ");
	scanf_s("%d", &n);
	int* a = create_arr(n);
	fill_arr(a, n);
	
	int m; 
	printf("Enter the length of the array B: ");
	scanf_s("%d", &m);
	int* b = create_arr(m);
	fill_arr(b, m);

	int k = isnondecr(a, n) + isnondecr(b, m);
	printf("k = %d\n", k);

	delete_arr(a);
	delete_arr(b);

	return 0;
}
