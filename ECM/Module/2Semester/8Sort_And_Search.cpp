#include <stdio.h>
#include "funks_header.h"
#include <limits>

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

		// printIntArr(arr, n);
	}
}

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

		// printIntArr(arr, n);
	}
}

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

		// printIntArr(arr, n);
	}
}

void qSort_r(int* arr, int lft, int rght) {
	if (lft >= rght) {
		// printIntArr(arr, 10);
		return;
	}

	int i = lft, j = rght;
	int x = arr[(i + j) / 2];
	int t;

	while (i <= j) {
		while (arr[i] < x && i <= rght) i++;
		while (arr[j] >= x && j >= lft) j--;

		if (i < j) {
			t = arr[j];
			arr[j] = arr[i];
			arr[i] = t;
			i++; j--;
		}
	}

	// printIntArr(arr, 10);

	if (i == lft && j < i) i++;
	if (j == rght && i > j) j--;

	if (lft <= j) qSort_r(arr, lft, j);
	if (i <= rght) qSort_r(arr, i, rght);
}

struct edges {
	int left;
	int right;
};

void qSort_i(int* arr, int n) {
	edges* b = new edges[n];
	int k = 0;
	b[k].left = 0; b[k].right = n - 1;

	int lft, rght, x, t, i, j;

	while (k >= 0) {
		lft = b[k].left; rght = b[k].right;
		k--;

		i = lft, j = rght;
		x = arr[(i + j) / 2];

		while (i <= j) {
			while (arr[i] < x && i <= rght) i++;
			while (arr[j] >= x && j >= lft) j--;

			if (i < j) {
				t = arr[j];
				arr[j] = arr[i];
				arr[i] = t;
				i++; j--;
			}
		}

		// printIntArr(arr, n);

		if (i == lft && j < i) i++;
		if (j == rght && i > j) j--;

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
	bubbleSort(arr, n);
}

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

		// printIntArr(arr, n);
	}
}

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

		// printIntArr(arr, n);
		// printIntArr(sorted, i + 1);
	}

	copyArray(arr, sorted, n);

	delete[] mins;
	delete[] sorted;
}

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

void mergeSort(int* arr, int lft, int rght) {
	if (lft >= rght) {
		// printIntArr(arr, 10);
		return;
	}
	int mid = (lft + rght) / 2;
	mergeSort(arr, lft, mid);
	mergeSort(arr, mid + 1, rght);

	int* b = new int[rght - lft + 1];
	merge(&arr[lft], mid - lft + 1, &arr[mid + 1], rght - mid, b);
	copyArray(&arr[lft], b, rght - lft + 1);
	delete[] b;

	// printIntArr(arr, 10);
}

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

		// printIntArr(arr, n);
	}

	delete[] b;
}

int main() {
	int a[10] = { 3, 9, 4, 4, 8, 5, 12, 18, 17, 6 };
	printIntArr(a, 10);
	int help1[3] = { 9, 3, 1 };
	int help2[4] = { 7, 5, 3, 2 };

	// insSort(a, 10);
	// shellSort(a, 10, help1, 3);
	// bubbleSort(a, 10);
	// qSort_r(a, 0, 9);
	// qSort_i(a, 10);
	// brushSort(a, 10, help2, 4);
	// minSort(a, 10);
	// sqminSort(a, 10);
	// mergeSort(a, 0, 9);
	nat2MergeSort(a, 10);

	printIntArr(a, 10);

	return 0;
}