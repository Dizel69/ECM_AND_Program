// Некоторые сортировки через меню


#include <stdio.h>
#include "funks_header.h"
#include <limits>
#include <clocale>

const int ARR_SIZE = 10;

void printSorts() {
	int i = 1;
	printf("%d. простые вставки\n", i++);
	printf("%d. сортировка Шелла\n", i++);
	printf("%d. пузырьковая сортировка\n", i++);
	printf("%d. рекурсивная быстрая сортировка\n", i++);
	printf("%d. нерекурсивная быстрая сортировка\n", i++);
	printf("%d. сортировка расчёской\n", i++);
	printf("%d. сортировка простым выбором\n", i++);
	printf("%d. сортировка квадратичным выбором\n", i++);
	printf("%d. сортировка слиянием\n", i++);
	printf("%d. сортировка естественным двухпутевым слиянием\n", i++);
	printf("%d. выход\n", i++);
}

/* простые вставки */ // O(n^2)
void insSort(int* arr, int n) {
	int temp, j;
	for (int i = 1; i < n; i++) {
		temp = arr[i];
		j = i - 1;
		while (j >= 0 && arr[j] > temp) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = temp;

		printIntArr(arr, n);
	}
}

/* ввод последовательности для сортировки Шелла */
int scanfShellSortSeq(int* seq, int maxNum) {
	printf("введите последовательность для сортировки (признак конца ввода: 1):\n");

	int i = 0;
	scanf_s("%d", &seq[i]);
	while (seq[i] != 1 && i < maxNum) {
		i++;
		scanf_s("%d", &seq[i]);
	}

	return seq[i] == 1 ? i + 1 : 0;
}

/* сортировка Шелла */ // O(n * log(n)^2) - для последовательности 2^p * 3^q
void shellSort(int* arr, int n, int* seq, int t) {
	int h, b, j;
	for (int s = 0; s < t; s++) {
		h = seq[s];
		for (int i = h; i < n; i++) {
			b = arr[i];
			j = i - h;
			while (j >= 0 && arr[j] > b) {
				arr[j + h] = arr[j];
				j -= h;
			}
			arr[j + h] = b;
		}

		printIntArr(arr, n);
	}
}

/* пузырьковая сортировка */ // O(n^2)
void bubbleSort(int* arr, int n) {
	int B = n, t;
	while (B) {
		t = 0;
		for (int i = 0; i < n - 1; i++) {
			if (arr[i] > arr[i + 1]) {
				t = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = t;

				t = i;
			}
		}
		B = t;

		printIntArr(arr, n);
	}
}

/* рекурсивная быстрая сортировка */ // O(n * log(n))
void qSort_r(int* arr, int lft, int rght) {
	if (lft >= rght) return;

	int i = lft, j = rght;
	int x = arr[(i + j) / 2];
	int t;

	while (i <= j) {
		while (arr[i] < x && i <= rght) i++;
		while (arr[j] > x && j >= lft) j--;

		if (i < j) {
			if (arr[i] != arr[j]) {
				t = arr[j];
				arr[j] = arr[i];
				arr[i] = t;
			}

			i++; j--;
		}
		else if (i == j) {
			i++;
		}
	}

	printIntArr(arr, ARR_SIZE);

	if (lft <= j) qSort_r(arr, lft, j);
	if (i <= rght) qSort_r(arr, i, rght);
}

struct edges {
	int left;
	int right;
};

/* нерекурсивная быстрая сортировка */ // O(n* log(n))
void qSort_i(int* arr, int n) {
	edges* b = new edges[n];
	int k = 0;
	b[k].left = 0; b[k].right = n - 1;

	int lft, rght, x, t, i, j;

	while (k >= 0) {
		lft = b[k].left; rght = b[k].right;
		k--;
		if (lft >= rght) continue;

		i = lft, j = rght;
		x = arr[(i + j) / 2];

		while (i <= j) {
			while (arr[i] < x && i <= rght) i++;
			while (arr[j] > x && j >= lft) j--;

			if (i < j) {
				if (arr[i] != arr[j]) {
					t = arr[j];
					arr[j] = arr[i];
					arr[i] = t;
				}

				i++; j--;
			}
			else if (i == j) {
				i++;
			}
		}

		printIntArr(arr, n);

		if (lft <= j) {
			k++;
			b[k].left = lft; b[k].right = j;
		}
		if (i <= rght) {
			k++;
			b[k].left = i; b[k].right = rght;
		}
	}

	delete[] b;
}

/* ввод последовательности для сортировки расчёской */
int scanfBrushSortSeq(int* seq, int maxNum) {
	printf("введите последовательность для сортировки (признак конца ввода: 0):\n");

	int i = 0;
	scanf_s("%d", &seq[i]);
	while (seq[i] != 0 && i < maxNum) {
		i++;
		scanf_s("%d", &seq[i]);
	}

	return i;
}

/* сортировка расчёской */ // O(n * log(n)^2) - для последовательности 2^p * 3^q
void brushSort(int* arr, int n, int* seq, int t) {
	int h, temp;
	for (int s = 0; s < t; s++) {
		h = seq[s];
		for (int i = 0; i < n - h; i++) {
			if (arr[i] > arr[i + h]) {
				temp = arr[i];
				arr[i] = arr[i + h];
				arr[i + h] = temp;
			}
		}

		printIntArr(arr, n);
	}

	printf("------------\n");
	bubbleSort(arr, n);
}

/* сортировка простым выбором */ // O(n^2)
void minSort(int* arr, int n) {
	int k, t;
	for (int i = 0; i < n; i++) {
		k = i;
		for (int j = i + 1; j < n; j++) {
			if (arr[j] < arr[k]) k = j;
		}
		t = arr[i];
		arr[i] = arr[k];
		arr[k] = t;

		printIntArr(arr, n);
	}
}

/* возвращает floor(sqrt(n)) */
int Isqrt(int n) {
	int pr = 0;
	for (int i = 1; i <= n; i++) {
		if (i * i > n) return pr;
		pr++;
	}

	return 0;
}

struct lclmin {
	int value;
	int global_index;
};

/* миннимум в массиве между индексами lft и rght */
int min(int* arr, int n, int lft, int rght, int& index) {
	int value = arr[lft];
	index = lft;
	for (int i = lft + 1; i < rght && i < n; i++) {
		if (arr[i] < value) {
			value = arr[i];
			index = i;
		}
	}

	return value;
}

/* минимум в массиве типа lclmin; критерий: lclmin.value */
void min(lclmin* arr, int n, int& index) {
	int val = arr[0].value;
	index = 0;
	for (int i = 1; i < n; i++) {
		if (arr[i].value < val) {
			val = arr[i].value;
			index = i;
		}
	}
}

/* сортировка квадратичным выбором */ // O(n^(3 / 2))
void sqminSort(int* arr, int n) {
	int p = Isqrt(n);
	int q = p * p == n ? p : p + 1;
	int i, blockMinInd, minOfMinsInd;

	lclmin* mins = new lclmin[q];
	int* sorted = new int[n];

	for (i = 0; i < q; i++) {
		mins[i].value = min(arr, n, i * p, (i + 1) * p, blockMinInd);
		mins[i].global_index = blockMinInd;
	}

	for (i = 0; i < n; i++) {
		min(mins, q, minOfMinsInd);
		sorted[i] = mins[minOfMinsInd].value;

		arr[mins[minOfMinsInd].global_index] = INT_MAX;

		mins[minOfMinsInd].value = min(arr, n, minOfMinsInd * p, (minOfMinsInd + 1) * p, blockMinInd);
		mins[minOfMinsInd].global_index = blockMinInd;

		printIntArr(arr, n);
		printIntArr(sorted, i + 1);
		printf("\n");
	}

	copyArray(arr, sorted, n);

	delete[] mins;
	delete[] sorted;
}

/* слияние массивов */
void merge(int* a, int n, int* b, int m, int* c) {
	int i = 0, j = 0, k = 0;
	while (i < n && j < m) {
		if (a[i] < b[j]) {
			c[k] = a[i]; i++;
		}
		else {
			c[k] = b[j]; j++;
		}
		k++;
	}

	if (i < n) {
		copyArray(&c[k], &a[i], n + m - k);
	}
	if (j < m) {
		copyArray(&c[k], &b[j], n + m - k);
	}
}

/* сортировка слиянием */ // O(n* log(n))
void mergeSort(int* arr, int lft, int rght) {
	if (lft >= rght) return;

	int mid = (lft + rght) / 2;
	mergeSort(arr, lft, mid);
	mergeSort(arr, mid + 1, rght);

	int* b = new int[rght - lft + 1];
	merge(&arr[lft], mid - lft + 1, &arr[mid + 1], rght - mid, b);
	copyArray(&arr[lft], b, rght - lft + 1);
	delete[] b;

	printIntArr(arr, ARR_SIZE);
}

/* сортировка естественным двухпутевым слиянием */ // O(n* log(n))
void nat2MergeSort(int* arr, int n) {
	int f = 0, k, i, j;
	int* b = new int[n];
	while (!f) {
		f = 1; k = 0;
		while (k < n) {
			i = k;
			while (i < n - 1 && arr[i] <= arr[i + 1]) i++;
			j = i + 1;
			while (j < n - 1 && arr[j] <= arr[j + 1]) j++;

			if (i < n - 1) {
				merge(&arr[k], i - k + 1, &arr[i + 1], j - i, &b[k]);
				f = 0; k = j + 1;
			}
			else {
				copyArray(&b[k], &arr[k], n - k);
				k = n;
			}
		}

		copyArray(arr, b, n);

		printIntArr(arr, n);
	}

	delete[] b;
}


int main() {
	setlocale(LC_ALL, "");
	int n = ARR_SIZE;
	// printf("number of elements: "); scanf_s("%d", &n);
	int* a = new int[n];
	printf("fill the array (%d elements)\n", n); scanfArr(a, n);
	int* b = new int[n];
	bool cycle = true;
	int nSort, x;
	printSorts();
	while (cycle) {
		printf("номер операции: "); scanf_s("%d", &nSort);
		printf("\n");
		switch (nSort) {
		case 1: {
			copyArray(b, a, n);
			printf("source: "); printIntArr(b, n);
			printf("\n");
			insSort(b, n);
			printf("\nresult: "); printIntArr(b, n);
			break;
		}
		case 2: {
			int* c = new int[n];
			int t = scanfShellSortSeq(c, n);
			copyArray(b, a, n);
			printf("\nsource: "); printIntArr(b, n);
			printf("\n");
			shellSort(b, n, c, t);
			printf("\nresult: "); printIntArr(b, n);
			delete[] c;
			break;
		}
		case 3: {
			copyArray(b, a, n);
			printf("source: "); printIntArr(b, n);
			printf("\n");
			bubbleSort(b, n);
			printf("\nresult: "); printIntArr(b, n);
			break;
		}
		case 4: {
			copyArray(b, a, n);
			printf("source: "); printIntArr(b, n);
			printf("\n");
			qSort_r(b, 0, n - 1);
			printf("\nresult: "); printIntArr(b, n);
			break;
		}
		case 5: {
			copyArray(b, a, n);
			printf("source: "); printIntArr(b, n);
			printf("\n");
			qSort_i(b, n);
			printf("\nresult: "); printIntArr(b, n);
			break;
		}
		case 6: {
			int* c = new int[n];
			int t = scanfBrushSortSeq(c, n);
			copyArray(b, a, n);
			printf("\nsource: "); printIntArr(b, n);
			printf("\n");
			brushSort(b, n, c, t);
			printf("\nresult: "); printIntArr(b, n);
			delete[] c;
			break;
		}
		case 7: {
			copyArray(b, a, n);
			printf("source: "); printIntArr(b, n);
			printf("\n");
			minSort(b, n);
			printf("\nresult: "); printIntArr(b, n);
			break;
		}
		case 8: {
			copyArray(b, a, n);
			printf("source: "); printIntArr(b, n);
			printf("\n");
			sqminSort(b, n);
			printf("\nresult: "); printIntArr(b, n);
			break;
		}
		case 9: {
			copyArray(b, a, n);
			printf("source: "); printIntArr(b, n);
			printf("\n");
			mergeSort(b, 0, n - 1);
			printf("\nresult: "); printIntArr(b, n);
			break;
		}
		case 10: {
			copyArray(b, a, n);
			printf("source: "); printIntArr(b, n);
			printf("\n");
			nat2MergeSort(b, n);
			printf("\nresult: "); printIntArr(b, n);
			break;
		}
		case 11: {
			cycle = false;
			break;
		}
		default: {
			printf("something went wrong...\n");
			break;
		}
		}
	}
	delete[] a;
	delete[] b;
	return 0;

}