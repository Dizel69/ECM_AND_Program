//1.3.	Следом квадратной матрицы называется сумма элементов, расположенных на главной диагонали. Дана квадратная матрица А порядка n и натуральное число m. По матрице построить вектор B размерности m, где Bi равно следу матрицы Аi , здесь   i=1, 2, … , m. 
//          В программе описать следующие функции :
//          -нахождение следа матрицы;
//          -умножение двух матриц;
//          -построение вектора по матрице.


#include <iostream>

int** GetMatrix(int n);
void FillMatrix(int** matrix, int n);
void PrintVector(int* arr, int n);
void RemoveMatrix(int** matrix, int n);
void fillMatrixWithZero(int** matrix, int n);
void CopyMatrix(int** arr1, int** arr2, int n);
int getTraceMatrix(int** matrix, int n);
int** getMatrixMulti(int** matrix, int** newMatrix, int n);
void FillVector(int* vector, int** matrix, int** newMatrix, int m, int n);

int main()
{
	int n, m;

	printf("Vvod n = ");
	scanf_s("%d", &n);
	printf("Vvod m = ");
	scanf_s("%d", &m);

	int** matrix = GetMatrix(n);
	int** newMatrix = GetMatrix(n);
	int* vector = new int[m];

	FillMatrix(matrix, n);
	CopyMatrix(matrix, newMatrix, n);

	FillVector(vector, matrix, newMatrix, m, n);
	PrintVector(vector, n);

	RemoveMatrix(newMatrix, n);
	RemoveMatrix(matrix, n);
	delete[] vector;
	return 0;
}

int** GetMatrix(int n)
{
	int** matrix = new int* [n];

	for (int i = 0; i < n; i++)
		matrix[i] = new int[n];

	return matrix;
}

void FillMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			scanf_s("%d", &matrix[i][j]);
}

void PrintVector(int* arr, int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

void RemoveMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
		delete[] matrix[i];

	delete[] matrix;
}

void fillMatrixWithZero(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0;
}

void CopyMatrix(int** arr1, int** arr2, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			arr2[i][j] = arr1[i][j];
}

int getTraceMatrix(int** matrix, int n)
{
	int trace = 0;

	for (int i = 0; i < n; i++)
		trace += matrix[i][i];

	return trace;
}

int** getMatrixMulti(int** arr1, int** arr2, int n)
{
	int** newMatrix = GetMatrix(n);
	fillMatrixWithZero(newMatrix, n);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				newMatrix[i][j] += arr1[i][k] * arr2[k][j];

	return newMatrix;
}

void FillVector(int* vector, int** matrix, int** newMatrix, int m, int n)
{
	for (int i = 0; i < m; i++)
	{
		vector[i] = getTraceMatrix(matrix, n);
		matrix = getMatrixMulti(matrix, newMatrix, n);
	}
}