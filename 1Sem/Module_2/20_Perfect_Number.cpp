//№20 Дан целочисленный массив размерности n. Определить является ли
//    последовательность элементов, находящаяся между первым элементом массива,
//    являющегося совершенным числом, и последним элементом массива
//    являющегося совершенным числом, арифметической прогрессией.Выдать
//    сообщение, если в массиве нет такой последовательности.

#include <iostream>
using namespace std;

int* create_arr(int n) {
	return new int[n];
}
void fill_arr(int* arr, int n) {
	for (int i = 0; i < n; i++) {
		scanf_s("%d", arr++);
	}
}

bool isperfect(int n) {                // проверка числа на совершенность
	int hn = n / 2, sum = 0;
	for (int i = 1; i <= hn; i++) {
		if (!(n % i)) sum += i;
	}

	return sum == n;
}

void find_sovs(int* arr, int n, int& fst, int& lst) {       // нахождение индексов первого и последнего совершенных чисел
	int i;
	fst = lst = 0;
	for (i = 0; i < n; i++) {
		if (isperfect(arr[i])) {
			fst = i;
			break;
		}
	}
	for (i = n - 1; i >= 0; i--) {
		if (isperfect(arr[i])) {
			lst = i;
			return;
		}
	}
	return;
}

int* slice(int* arr, int fst, int lst, int& newlen) {          // получение среза и его длины
	newlen = lst - fst - 1;
	int* sl = new int[lst - fst - 1];
	int i, j;
	for (i = fst + 1, j = 0; i < lst; i++, j++) {
		sl[j] = arr[i];
	}
	return sl;
}

bool check_prog(int* arr, int n) {     // проверка на арифметическую прогрессию
	int d = arr[1] - arr[0];
	for (int i = 1; i < n - 1; i++) {
		if (arr[i + 1] - arr[i] != d) return false;
	}
	return true;
}
int main() {
	int n; scanf_s("%d", &n);
	int* arr = create_arr(n);
	fill_arr(arr, n);

	int fst, lst;
	find_sovs(arr, n, fst, lst);

	if (lst - fst > 2) {
		int nlen;
		int* slc = slice(arr, fst, lst, nlen);
		if (check_prog(slc, nlen)) {
			printf("It's a progression\n");
		}
		else {
			printf("It's not a progression\n");
		}
	}
	else {
		printf("No sequence\n");
	}



	return 0;
}
