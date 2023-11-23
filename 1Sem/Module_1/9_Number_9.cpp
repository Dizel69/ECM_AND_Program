//№9 Дано натуральное число n. Определить, есть ли хотя бы одна цифра 9 в записи
//   числа.

#include <iostream>
using namespace std;

int main()
{
    int n, m, ost;

    printf("Enter the number: ");
    scanf_s("%d", &n);
    m = n;

    while (m > 0)
    {
        ost = m % 10;

        if (ost == 9)
            break;

        m /= 10;
    }

    if (ost == 9)
        printf("Number %d contains 9", n);
    else
        printf("Number %d does not contains 9", n);

    return 0;
}