﻿// 1.1 Найти все натуральные числа, меньшие заданного числа N, при делении
//     на которые число N дает остаток в виде степени тройки.


#include <stdio.h>
#include "funks_header.h"

bool ispow(int x, int base) {
	if (x > 0) while (!(x % base)) x /= base;

	return x == 1;
}

int f11(int* result, int N) {
	int curpos = 0;
	for (int i = 1; i < N; i++) {
		if (ispow(N % i, 3)) result[curpos++] = i;
	}
	result[curpos] = 0;

	return curpos;
}

int gcd(int x, int y) {
	while (x && y) {
		if (x > y) x %= y;
		else y %= x;
	}

	return x + y;
}

int f12(int* result, int N) {
	int hN = N / 2, curpos = 0;
	for (int i = 1; i <= hN; i++) {
		if (gcd(i, N - i) == 1) result[curpos++] = i;
	}
	result[curpos] = 0;

	return curpos;
}

int trace(int** sqMatrix, int n) {
	int result = 0;
	for (int i = 0; i < n; i++) {
		result += sqMatrix[i][i];
	}

	return result;
}

int* f13_crVector(int** sqMatrix, int n, int maxPower) {
	int* vector = new int[maxPower];
	int** help;
	int** sqMatrixPow = newMatrix(n, n);

	int i = 0;
	vector[i++] = trace(sqMatrix, n);
	copyMatrix(sqMatrixPow, sqMatrix, n, n);

	for (i; i < maxPower; i++) {
		help = prodMatrix(sqMatrixPow, sqMatrix, n, n, n);
		vector[i] = trace(help, n);
		copyMatrix(sqMatrixPow, help, n, n);

		deleteMatrix(help, n);
	}

	deleteMatrix(sqMatrixPow, n);

	return vector;
}

int main() {
	int N, sizeArr; scanf_s("%d", &N);
	int* f1arr = new int[N];
	sizeArr = f11(f1arr, N);

	printIntArr(f1arr, sizeArr);
	delete[] f1arr;
	return 0;
}


