// 3.1  Написать программу, которая проверяет работу функций со строками в
//      диалоговом режиме с помощью меню.

#include <iostream>
#include <stdio.h>
#include "str_header.h"
#include "const.h"

void printMenu() {
	printf("1. Ввести строки\n");
	printf("2. Подсчитать длину\n");
	printf("3. Конкатенация\n");
	printf("4. Копировать\n");
	printf("5. Поиск символа\n");
	printf("6. Поиск подстроки\n");
	printf("7. Сравнить\n");
	printf("8. Вывести строку\n");
	printf("9. Выход\n");
}


int main() {
	setlocale(LC_ALL, "ru");
	char* str1 = new char[STRING_SIZE];
	char* str2 = new char[STRING_SIZE];
	char* substr = new char[STRING_SIZE];
	char chr;
	int nFunc;
	bool cycle = true;
	printMenu();
	while (cycle) {
		scanf_s("%d", &nFunc);
		scanf_s("%c", &chr);
		switch (nFunc) {
		case 1: {
			gets_s(str1, STRING_SIZE);
			gets_s(str2, STRING_SIZE);
			printf("ok\n");
			break;
		}
		case 2: {
			printf("Первая строка: %d\n", strlen_a(str1, STRING_SIZE));
			printf("Вторая строка: %d\n", strlen_a(str2, STRING_SIZE));
			break;
		}
		case 3: {
			if (!strcat_a(str1, STRING_SIZE, str2, STRING_SIZE)) {
				printf("Что-то пошло не так...\n");
			}
			else {
				printf("ok\n");
			}
			break;
		}
		case 4: {
			if (!strcpy_a(str1, STRING_SIZE, str2, STRING_SIZE)) {
				printf("Что-то пошло не так...\n");
			}
			else {
				printf("ok\n");
			}
			break;
		}
		case 5: {
			printf("Введите символ: "); scanf_s("%c", &chr);
			char* chr1 = strchr_a(str1, STRING_SIZE, chr);
			char* chr2 = strchr_a(str2, STRING_SIZE, chr);
			if (chr1) {
				printf("%c находится на позиции: %d в str1\n", chr, (int)(chr1 - str1));
			}
			else {
				printf("Нет символа в str1\n");
			}
			if (chr2) {
				printf("%c находится на позиции: %d в str2\n", chr, (int)(chr2 - str1));
			}
			else {
				printf("Нет символа в str2\n");
			}
			break;
		}
		case 6: {
			printf("Введите подстроку: "); gets_s(substr, STRING_SIZE);
			char* sub1 = strstr_a(str1, STRING_SIZE, substr, STRING_SIZE);
			char* sub2 = strstr_a(str2, STRING_SIZE, substr, STRING_SIZE);
			if (sub1) {
				printf("%s находится на позиции %d в str1\n", substr, (int)(sub1 - str1));
			}
			else {
				printf("Нет  str1\n");
			}
			if (sub2) {
				printf("%s находится на позиции %d в str2\n", substr, (int)(sub2 - str1));
			}
			else {
				printf("Нет подстрок в str2\n");
			}
			break;
		}
		case 7: {
			int cmp = strcmp_a(str1, STRING_SIZE, str2, STRING_SIZE);
			if (!cmp) {
				printf("Что-то пошло не так...\n");
			}
			else {
				printf("Разница между {%s} и {%s}: %d\n", str1, str2, cmp);
			}
			break;
		}
		case 8: {
			puts(str1);
			puts(str2);
			break;
		}
		case 9: {
			cycle = false;
			break;
		}
		default:
			printf("Неправильный номер\n");
			break;
		}
	}
	delete[] str1;
	delete[] str2;
	delete[] substr;
}