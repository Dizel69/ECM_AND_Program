// 2 Контрольная 2 задача
//     Дана целочисленная матрица размера n x m. Построить вектор в1, в2, ..., вk из номеров строк, которые являются полиндромами.

#include <iostream>

int** Create_Matr(int n, int m){
	int** matr = new int* [n];
	for (int i = 0; i < m; i++)
		matr[i] = new int[m];

	return matr;
}

void Fill_Matr(int** matr, int n, int m){
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			scanf_s("%d", &matr[i][j]);
	}
}

void Delete_Matrix(int** matr, int n, int m){
	for (int i = 0; i < n; i++)
		delete[] matr[i];

	delete[] matr;
}

bool Check_Str(int* arr, int m){
	int l, k;

	l = m / 2;
	k = m - 1;

	for (int j = 0; j < l; j++){
		if (arr[j] == arr[k])
			return true;
		else{
			return false;
			break;
		}

		k--;
	}
}

int main() {
	int n;
	printf("Enter n: ");
	scanf_s("%d", &n);

	int m;
	printf("Enter m: ");
	scanf_s("%d", &m);

	int** matr = Create_Matr(n, m);

	Fill_Matr(matr, n, m);

	for (int i = 0; i < n; i++)
		if (Check_Str(matr[i], m))
			printf("%d ", i);

	Delete_Matrix(matr, n, m);

	return 0;
}