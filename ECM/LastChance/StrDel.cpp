// Написать функцию strdel(char *s, int k, int m), которая удаляет из строки s с k элемента m символов
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>



void strdel(char* s, int k, int m) {
    if (k >= strlen(s) || k < 0) return;//защита от некорректного p
    if (m + k > strlen(s))//защита от выхода за пределы массива
        m = strlen(s) - k;
    char str[80] = "\0";
    int i = 0, border = k+m;
    for (int j = 0; s[j] != '\0'; j++)
    {
        if (j < k || j > border)
        {
            str[i] = s[j];
            i++;
        }
    }
    str[i] = '\0';
    strcpy(s, str);
}

int main() {
    char s[80] ="\0";
    int k, m;
    printf("Enter the string:\n");
    scanf("%s", s);
    
   
    printf("Enter the element to start with:\n");
    scanf_s("%d", &k);
    
    printf("Enter the number of items you want to delete:\n");
    scanf_s("%d", &m);
    strdel(s, k, m);
    printf("strdel: %s\n", s);
    return 0;
}