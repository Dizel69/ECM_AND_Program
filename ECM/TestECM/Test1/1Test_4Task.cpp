//1 Контрольная 4 задание
//     Дан целочисленный массив из n элементов. Поменять порядок следования элементов, находящихся 
//     между первым и последним положительными элементами массива, на обратный. 
//     Если такая операция не может быть выполнена, то выдать сообщение.

#include <iostream>

void Fill_Arr(int* arr, int n){         //Заполняет массив
	for (int i = 0; i < n; i++)
		scanf_s("%d", &arr[i]);
}

int Find_FirstPosit(int* arr, int n){        //Находит первое положительное число
	for (int i = 0; i < n; i++)
		if (arr[i] > 0)
			return i;

	return 0;
}

int Find_LastPosit(int* arr, int n){     //Находит последнее положительное число
	for (int i = n - 1; i >= 0; i--)
		if (arr[i] > 0)
			return i;

	return 0;
}

int main(){
	int n;
	printf("Enter the length of the array: ");
	scanf_s("%d", &n);
	int PervPol, PoslPol, m, a, t;
	int* arr = new int[n];

	Fill_Arr(arr, n);
	PervPol = Find_FirstPosit(arr, n);
	PoslPol = Find_LastPosit(arr, n);


	a = PoslPol - 1;
	m = PervPol + 1;
	for (int i = m; i <= a; i++){
		t = arr[i];
		arr[i] = arr[a];
		arr[a] = t;
		a--;
	}

	for (int i = 0; i < n; i++)
		printf("%d", arr[i]);

	delete[] arr;
	return 0;
}

