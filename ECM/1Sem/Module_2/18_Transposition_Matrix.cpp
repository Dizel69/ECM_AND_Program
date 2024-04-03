//№18 Написать программу транспонирования квадратной матрицы:
//      а) результат транспонирования направить в другую матрицу;
//      б) результат транспонирования получить на месте исходной матрицы.

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
void print_matrix(int** mat, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
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
void n18b_trans_n_stay(int** a, int n) {
	int t;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			t = a[i][j];
			a[i][j] = a[j][i];
			a[j][i] = t;
		}
	}
}
int main() {
	int n; scanf_s("%d", &n);
	int** a = create_matrix(n, n);
	fill_matrix(a, n, n);

	int** aT = n18a_trans_n_copy(a, n, n);
	n18b_trans_n_stay(a, n);

	printf("\n");
	printf("transponented matrix (a):\n");
	print_matrix(aT, n, n);

	printf("transponented matrix (b):\n");
	print_matrix(a, n, n);

	

	return 0;
}