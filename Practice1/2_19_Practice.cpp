// 2 Вариант 19 задание
//         Дана  целочисленная  квадратная  матрица  размером  n*n.  Вычислить (x1+xn)*(x2+xn-1)* …*(xn+x1),
//         где  xk  -  среднее  геометрическое  из  элементов  k-й  строки  данной  матрицы.

#include <iostream>


int** Get_Matr(int n);
void Fill_Matr(int** a, int n);
void Remove_Matr(int** matr, int n);
int Geo_Mean(int* arr, int n);
int my_pow(int res, int n);
int MPNH(int** matr, int n);

int main(){
	int n = 4;
	int** matr = Get_Matr(n);

	Fill_Matr(matr, n);

	printf("%d", MPNH(matr, n));

	Remove_Matr(matr, n);

	return 0;
}

int** Get_Matr(int n){
	int** matr;

	matr = new int* [n];
	for (int i = 0; i < n; i++)
		matr[i] = new int[n];

	return matr;
}

void Fill_Matr(int** matr, int n){
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			scanf_s("%d", &matr[i][j]);
}

void Remove_Matr(int** matr, int n){
	for (int i = 0; i < n; i++)
		delete[] matr[i];

	delete[] matr;
}

int Geo_Mean(int* arr, int n){
	int res = 1;

	for (int i = 0; i < n; i++)
		res *= arr[i];

	return my_pow(res, n);
}

int my_pow(int res, int n){
	int result = 1, z = 1;

	for (int i = 0; i < n; i++){
		result = result * z;

		if (result == res)
			return z;
		else
			z++;
	}
}

int MPNH(int** matr, int n){
	int res = 1, k = n;
	for (int i = 0; i < n; i++){
		res *= Geo_Mean(matr[i], n) + Geo_Mean(matr[k - 1], n);
		k--;
	}
	return res;
}