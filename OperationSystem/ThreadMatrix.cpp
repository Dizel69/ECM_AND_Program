#include <iostream>
#include <thread>
#include <ctime>
using namespace std;


const int NumberThreads = 10;
int from[NumberThreads] = { 0 }, to[NumberThreads] = { 0 };

int** CreateMatrix(int rows, int colums)
{
	int** arr = new int* [rows];
	for (int i = 0; i < rows; i++)
		arr[i] = new int[colums];

	return arr;
}

void DeleteMatrix(int** matr, int rows, int colums)
{
	for (int i = 0; i < colums; i++)
		delete[] matr[i];

	delete[] matr;
}

void FillArr(int** arr, int n, int m)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			arr[i][j] = rand();
		}
	}
}

void mulMat(int** matr_1, int** matr_2, int** matr_3, int rows_1, int colums_1, int rows_2, int colums_2)
{
	for (int i = 0; i < rows_1; i++) {
		for (int j = 0; j < colums_2; j++) {
			matr_3[i][j] = 0;

			for (int k = 0; k < rows_2; k++) {
				matr_3[i][j] += matr_1[i][k] * matr_2[k][j];
			}
		}
	}
}

void ThreadFunctionMult(void* param, int** matr_1, int** matr_2, int** matr_3, int k, int m)
{
	int iNum = *(reinterpret_cast<int*>(param));
	for (int i = from[iNum]; i < to[iNum]; i++)
	{
		for (int j = 0; j < k; j++)
		{
			matr_3[i][j] = 0;
			for (int z = 0; z < m; z++)
				matr_3[i][j] += matr_1[i][z] * matr_2[z][j];
		}
	}
}

void PrintArr(int** arr, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++) {
			cout << arr[i][j] << "\t";
		}
		cout << "\n";
	}
}


int main()
{
	setlocale(0, "rus");

	int rows_1, colums_1, rows_2, colums_2;

	cout << "Введите размерность первой матрицы: ";
	cin >> rows_1 >> colums_1;

	cout << "Введите размерность второй матрицы: ";
	cin >> rows_2 >> colums_2;

	

	int q = rows_1 / NumberThreads;
	int r = rows_1 % NumberThreads;

	int** FirstMatrix = CreateMatrix(rows_1, colums_1);
	int** SecondMatrix = CreateMatrix(rows_2, colums_2);
	int** FirstRes = CreateMatrix(rows_1, colums_2);
	int** SecondRes = CreateMatrix(rows_1, colums_2);

	FillArr(FirstMatrix, rows_1, colums_1);
	FillArr(SecondMatrix, rows_2, colums_2);

	for (int i = 0; i < rows_1; i++) {
		for (int j = 0; j < colums_2; j++) {
			SecondRes[i][j] = 0;
		}
	}

	// Первая матрица
	clock_t start_time_1, end_time_1;
	start_time_1 = clock();
	mulMat(FirstMatrix, SecondMatrix, FirstRes, rows_1, colums_1, rows_2, colums_2);
	end_time_1 = clock() - start_time_1;
	std::cout << "Время вычисления без потоков: " << 1000 * (end_time_1 / (double)CLOCKS_PER_SEC) << " миллисикунд." << std::endl;

	// Вторая матрица
	clock_t start_time_2, end_time_2;
	start_time_2 = clock();

	thread threads[NumberThreads];
	int noms[NumberThreads];

	for (int i = 0; i < NumberThreads; i++) {
		to[i] = from[i] + q + (i < r ? 1 : 0);
		noms[i] = i;
		threads[i] = thread(ThreadFunctionMult, (void*)(noms + i), FirstMatrix, SecondMatrix, SecondRes, colums_2, rows_2);
		if (i < NumberThreads - 1)
			from[i + 1] = to[i];
	}

	for (int i = 0; i < NumberThreads; i++)
	{
		threads[i].join();
	}

	end_time_2 = clock() - start_time_2;
	std::cout << "Время вычисления с потоками: " << 1000 * (end_time_2 / (double)CLOCKS_PER_SEC) << " миллисикунд." << std::endl;


	DeleteMatrix(FirstMatrix, rows_1, colums_1);
	DeleteMatrix(SecondMatrix, rows_2, colums_2);
	DeleteMatrix(FirstRes, rows_1, colums_2);
	DeleteMatrix(SecondRes, rows_1, colums_2);

	return 0;
}


