//№24 Даны две вещественные матрицы А и В размерности n x m. Построить матрицу С,
//    где элемент С[i][j] равен сумме элементов i–й строки матрицы А, которые
//    отсутствуют в j–ом столбце матрицы В, и нулю в противном случае.


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
void delete_matrix(int** mat, int n, int m) {
	for (int i = 0; i < n; i++) {
		delete[] mat[i];
	}
	delete[] mat;
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

int* vcol(int** mat, int n, int j) {       // создание вектора-столбца
	int* col = new int[n];
	for (int i = 0; i < n; i++) {
		col[i] = mat[i][j];
	}
	return col;
}

bool isthere(int* arr, int n, int num) {        // поиск числа в массиве
	for (int i = 0; i < n; i++) {
		if (arr[i] == num) return true;
	}

	return false;
}

int calcCij(int* row, int n, int* col, int m) {         // подсчёт элемента для новой матрицы
	int cij = 0;
	for (int i = 0; i < n; i++) {
		if (!isthere(col, m, row[i])) cij += row[i];
	}

	return cij;
}

int** createC(int** A, int** B, int n, int m) {  // создание новой матрицы
	int** C = create_matrix(n, m);
	int* row; int* col;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			row = A[i];
			col = vcol(B, n, j);
			C[i][j] = calcCij(row, m, col, n);

		}
	}

	return C;
}
int main() {
	int n, m; scanf_s("%d", &n); scanf_s("%d", &m);
	int** A = create_matrix(n, m);
	int** B = create_matrix(n, m);
	printf("enter the matrix:\n");
	fill_matrix(A, n, m);
	printf("enter the matrix:\n");
	fill_matrix(B, n, m);

	int** C = createC(A, B, n, m);
	printf("new matrix:\n");
	print_matrix(C, n, m);

	delete_matrix(A, n, m);
	delete_matrix(B, n, m);
	delete_matrix(C, n, m);

	return 0;
}
