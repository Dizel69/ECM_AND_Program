//№22 Дана квадратная вещественная матрица размерности n x n. Вычислить:
//    -сумму элементов, лежащих на главной диагонали :
//    -сумму элементов, лежащих на побочной диагонали;
//    -сумму элементов, лежащих выше главной диагонали;
//    -сумму элементов, лежащих ниже главной диагонали.

#include <iostream>
using namespace std;

int** CreateMatrix(int n, int m) {
	int** matrix = new int* [n];

	for (int i = 0; i < m; i++)
		matrix[i] = new int[m];
	return matrix;
}
int** Fill(int** matrix, int n, int m) {
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("Enter the matrix: ");
			scanf_s("%d", &matrix[i][j]);
		}
	}

	return matrix;
}
void Show_Matrix(int** matrix, int n, int m) {
	
	for (int i = 0; i < n; i++) {
		printf("| ");
		for (int j = 0; j < m; j++)
			printf("%d | ", matrix[i][j]);
		printf("\n");
	}
}
int GenSumDiag(int** matrix, int n, int m) {
	int sum = 0;

	for (int i = 0; i < n; i++) {
		sum += matrix[i][i];
	}

	return sum;
}
int AltSumDiag(int** matrix, int n, int m) {
	int sum = 0, cursed = 0;
	
	for (int i = n - 1; i >= 0; i--, cursed++)
		sum += matrix[i][cursed];
	
	return sum;
}
int HighGenSumDiag(int** matrix, int n, int m) {
	int sum = 0, cursed = 0;
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			if (j > i)
				sum += matrix[i][j];
	}

	return sum;
}
int LowGenSumDiag(int** matrix, int n, int m){
	int sum = 0, cursed = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (j < i)
				sum += matrix[i][j];

	return sum;
}

int main() {
	
	int n = 3, GS, AS, HS, LS;
	int** matrix = CreateMatrix(n, n);
	Fill(matrix, n, n);
	Show_Matrix(matrix, n, n);

	GS = GenSumDiag(matrix, n, n);
	AS = AltSumDiag(matrix, n, n);
	HS = HighGenSumDiag(matrix, n, n);
	LS = LowGenSumDiag(matrix, n, n);

	printf("Genereal Diagonal Sum = %d\nAlternative Diagonal Sum = %d\nHigher General Diagonal Sun = %d\nLower General Sum = %d\n", GS, AS, HS, LS);

	delete[] matrix;
	return 0;
}