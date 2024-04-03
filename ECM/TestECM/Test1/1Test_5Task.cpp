//1 Контрольная 5 задание
// Дан целочисленный массив из n элементов. Создать новый массив из повторяющихся элементов первого 
// записав их по одному разу, или выдать сообщение, что повторяющихся элементов нет

#include <iostream>

void Fill_Arr(int* arr, int n){
	for (int i = 0; i < n; i++)
		scanf_s("%d", &arr[i]);
}

bool Check_El(int* arr, int n, int el){
	for (int i = 0; i < n; i++)
		if (arr[i] == el)
			return true;

	return false;
}

int main(){
	int n;
	printf("Enter the length of the array: ");
	scanf_s("%d", &n);

	int k = 0;
	int* arr1 = new int[n];
	int* arr2 = new int[n] { 0 };
	Fill_Arr(arr1, n);

	for (int i = 0; i < n - 1; i++){
		for (int j = i + 1; j < n; j++){
			if (arr1[i] == arr1[j]){
				if (!Check_El(arr2, n, arr1[i])){
					arr2[k] = arr1[i];
					k++;
				}
				break;
			}
		}
	}

	if (k == 0)
		printf("There are no duplicate elements in the array");
	else
		for (int i = 0; i < k; i++)
			printf("%d ", arr2[i]);

	delete[] arr1;
	delete[] arr2;
	return 0;
}

