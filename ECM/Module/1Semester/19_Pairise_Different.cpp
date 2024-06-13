//№19 Дана прямоугольная матрица целых положительных чисел размерности n x m.
//    Найти количество строк и столбцов матрицы, в которых элементы попарно
//    различны.Если таких строк или столбцов нет, то выдать сообщение.

#include <iostream>
using namespace std;

int** create_matrix(int n, int m) {
	int** a = new int* [n];
	for (int i = 0; i < n; i++) {
		a[i] = new int[m];
	}
	return a;
}
void fill_matrix(int** mat, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf_s("%d", &mat[i][j]);
		}
	}
}
int** n18a_trans_n_copy(int** a, int n, int m) {
	int** b = create_matrix(m, n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			b[i][j] = a[j][i];
		}
	}
	return b;
}
int n19check_purmutation(int* arr, int n) {
	int C;
	for (int i = 0; i < n; i++) {
		C = arr[i];
		for (int j = i + 1; j < n; j++) {
			if (arr[j] == C) return 0;
		}
	}

	return 1;
}
int count_permutations(int** mat, int n, int m) {
	int i, count = 0;
	for (i = 0; i < n; i++) {
		count += n19check_purmutation(mat[i], m);
	}

	return count;
}
int main() {
	int n, m;
	scanf_s("%d", &n); scanf_s("%d", &m);
	int** mat = create_matrix(n, m);
	fill_matrix(mat, n, m);

	int** matT = n18a_trans_n_copy(mat, n, m);

	int c1 = count_permutations(mat, n, m), c2 = count_permutations(matT, m, n);
	printf("permutations in rows: %d\n", c1);
	printf("permutations in columns: %d\n", c2);
	printf("together: %d\n", c1 + c2);

	return 0;
}