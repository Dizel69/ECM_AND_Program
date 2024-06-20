/*Характеристикой строки целочисленной матрицы назовём сумму её положительных чётных элементов. Переставляя строки заданной матрицы, расположить
  их в соотвествии с ростом характеристик. Для сортировки использовать метод выбора. Исходные данные находятся в текстовом файле, первая строка 
  которого содержит два числа n и m.*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<iostream>

int** MatrixMemory(int n, int m) {
	int** matrix = new int* [n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[m];
	return matrix;
}

void FillMatrix(int** matrix, int n, int m, FILE* f) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			fscanf(f, "%d", &matrix[i][j]);
	}
}

void MatrixPrint(int** matrix, int n, int m) {
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			printf("%d ", matrix[i][j]);

		printf("\n");
	}
}

void MatrixDelete(int** matrix, int n) {
	for (int i = 0; i < n; i++)
		delete[] matrix[i];
	delete[] matrix;
}

void ArrayPrint(int* arr, int n) {
	for (int i = 0; i < n; i++)
		printf("%d ", arr[i]);
}

int ArrayChar(int* arr, int n) {
	int result = 0;
	for (int i = 0; i < n; i++) {
		if (arr[i] % 2 == 0 && arr[i] > 0) result += arr[i];
	}
	return result;
}

int* FillArrOfChars(int** matrix, int n, int m) {
	int* arr = new int[n];
	for (int i = 0; i < n; i++)
		arr[i] = ArrayChar(matrix[i], m);
	return arr;
}

void ElementSwap(int** matrix, int n, int m, int i, int j) {
	int t;
	for (int k = 0; k < m; k++) {
		t = matrix[i][k];
		matrix[i][k] = matrix[j][k];
		matrix[j][k] = t;
	}
}

void ChSort(int** matrix, int* arr, int n, int m) {
	int k, t1;

	for (int i = 0; i < n; i++) {
		k = i;
		for (int j = i + 1; j < n; j++) {
			if (arr[j] < arr[k]) k = j;
		}
		t1 = arr[i];
		arr[i] = arr[k];
		arr[k] = t1;

		ElementSwap(matrix, n, m, i, k);
	}
}

int main() {
	FILE* f = fopen("buba.txt", "r");

	int n, m;
	fscanf(f, "%d%d", &n, &m);

	int** matrix = MatrixMemory(n, m);
	FillMatrix(matrix, n, m, f);

	MatrixPrint(matrix, n, m); printf("\n");

	// массив характеристик каждой строки
	int* arrOfChars = FillArrOfChars(matrix, n, m);

	ArrayPrint(arrOfChars, n); printf("\n");
	printf("-----------------------------\n");
	ChSort(matrix, arrOfChars, n, m);

	MatrixPrint(matrix, n, m); printf("\n");
	ArrayPrint(arrOfChars, n); printf("\n");
}
