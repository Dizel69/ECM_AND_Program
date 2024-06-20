/*Матрицу размером M*N сортирует матрицу в зависимости от индекса строки. Если индекс строки чётный, то используется 
сортировка вставками. Если индекс - нечётный, то используется сортировка выбором.
*/

#include <iostream>
using namespace std;
 
void matrixGenerateValues(int **matr, int rows, int cols){
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            matr[i][j] = -100 + rand() % 201; // [-100..100]
        }
    }
}
 
void matrixPrint(int **matr, int rows, int cols)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            printf("%d\t", matr[i][j]);
        }
        printf("\n");
    }
}

void insertionSort(int* arrayPtr, int length) // сортировка вставками
{
    int temp, // временная переменная для хранения значения элемента сортируемого массива
        item; // индекс предыдущего элемента
    for (int counter = 1; counter < length; counter++)
    {
        temp = arrayPtr[counter]; // инициализируем временную переменную текущим значением элемента массива
        item = counter - 1; // запоминаем индекс предыдущего элемента массива
        while (item >= 0 && arrayPtr[item] > temp) // пока индекс не равен 0 и предыдущий элемент массива больше текущего
        {
            arrayPtr[item + 1] = arrayPtr[item]; // перестановка элементов массива
            arrayPtr[item] = temp;
            item--;
        }
    }
}

void choicesSort(int* arrayPtr, int length_array) // сортировка выбором
{
    for (int repeat_counter = 0; repeat_counter < length_array; repeat_counter++)
    {
        int temp = arrayPtr[0]; // временная переменная для хранения значения перестановки
        for (int element_counter = repeat_counter + 1; element_counter < length_array; element_counter++)
        {
            if (arrayPtr[repeat_counter] < arrayPtr[element_counter])
            {
                temp = arrayPtr[repeat_counter];
                arrayPtr[repeat_counter] = arrayPtr[element_counter];
                arrayPtr[element_counter] = temp;
            }
        }
    }
}
 
int main()
{
    int **matr = NULL, m, n;
    
    printf("Enter n:\n");
    scanf_s("%d", &n);

    printf("Enter m:\n");
    scanf_s("%d", &m);
    
    
    matr = new int*[m];
    for(int i = 0; i < m; i++)
    {
        matr[i] = new int[n];
    }
   
    matrixGenerateValues(matr, m, n);
    matrixPrint(matr, m, n);
    
    for (int i = 0; i < m; i++)
    {
        if (i % 2 == 0)
            insertionSort(matr[i], n);
        else
            choicesSort(matr[i], n);
    }
    
    printf("\n\n");
    matrixPrint(matr, m, n);
    
    
    for(int i = 0; i < m; i++)
    {
        delete[] matr[i];
    }
    delete[] matr;
 
    return 0;
}
