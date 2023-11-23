//№21 Дана квадратная вещественная матрица размерности n x n. Найти скалярное
//    произведение строки на столбец, на пересечении которых находится последний
//    минимальный элемент матрицы.


#include <iostream>

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

void delete_arr(int* arr) {
	delete[] arr;
}

int find_lst_min(int** mat, int n, int m, int& x, int& y) {    // нахождение последнего минимального элемента матрицы
	int min = INT_MAX;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (mat[i][j] <= min) {
				min = mat[i][j];
				x = i; y = j;
			}
		}
	}
	return min;
}

int* vcol(int** mat, int n, int j) {     // создание вектора-столбца
	int* col = new int[n];
	for (int i = 0; i < n; i++) {
		col[i] = mat[i][j];
	}
	return col;
}

int sc_prod(int* a, int* b, int n) {   // скалярное произведение
	int prod = 0;
	for (int i = 0; i < n; i++) {
		prod += a[i] * b[i];
	}
	return prod;
}
int main() {
	int n; scanf_s("%d", &n);
	int** mat = create_matrix(n, n);
	fill_matrix(mat, n, n);

	int x, y;
	int mn = find_lst_min(mat, n, n, x, y);
	int* row = mat[x];
	int* col = vcol(mat, n, y);

	printf("the smallest element is %d on place %d %d\n", mn, x, y);
	printf("scalar product of %d row and %d column is %d\n", x, y, sc_prod(row, col, n));

	delete_matrix(mat, n, n);
	delete_arr(col);

	return 0;
}
