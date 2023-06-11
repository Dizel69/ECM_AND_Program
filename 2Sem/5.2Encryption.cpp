// 5.2   На вход программе подается строка (длиной не более 250 символов), в которой нужно зашифровать все 
//       английские слова (словом называется непрерывная последовательность английских букв, слова друга от друга отделяются 
//       любыми другими символами, длина слова не превышает 15 символов).  Каждое слово зашифровано с помощью циклического
//       сдвига на длину этого слова. Например, если длина слова равна K, каждая буква в слове заменяется на букву, стоящую
//	     в английском алфавите на K букв дальше (алфавит считается циклическим, то есть, за буквой Z стоит буква A). Строчные 
//       буквы при этом остаются строчными, а прописные – прописными. Символы, не являющиеся английскими буквами, не изменяются.



#include <stdio.h>
#include <string.h>
#include "funks_header.h"
#include "const.h"

void printIntArr(int* arr, int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int** newMatrix(int n, int m) {
	int** matrix = new int* [n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new int[m];
	}

	return matrix;
}

void scanfMatrix(int** matrix, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf_s("%d", &matrix[i][j]);
		}
	}
}

void printMatrix(int** matrix, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

void deleteMatrix(int** matrix, int rows) {
	for (int i = 0; i < rows; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

int** prodMatrix(int** nm_Mat, int** mk_Mat, int n, int m, int k) {
	int** nk_Mat = newMatrix(n, k);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			nk_Mat[i][j] = 0;
			for (int l = 0; l < m; l++) {
				nk_Mat[i][j] += nm_Mat[i][l] * mk_Mat[l][j];
			}
		}
	}

	return nk_Mat;
}

void copyMatrix(int** Destination, int** Source, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			Destination[i][j] = Source[i][j];
		}
	}
}

void copyArray(int* Destination, int* Source, int n) {
	for (int i = 0; i < n; i++) {
		Destination[i] = Source[i];
	}
}
char* strcrt52(char* str) {
	char* pstr = str;
	int len = strlen(str);
	int move = len % 26;

	while (*str) {
		if (*str >= 'a' && *str <= 'z') {

			if (*str + move <= 'z') *str = *str + move;
			else *str = 'a' - 1 + *str + move - 'z';

		}
		else if (*str >= 'A' && *str <= 'Z') {

			if (*str + move <= 'Z') *str = *str + move;
			else *str = 'A' - 1 + *str + move - 'Z';

		}

		str++;
	}

	return pstr;
}

bool iseng(char chr) {
	return (chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z');
}

char* f52(char* text) {
	char word[WORD_SIZE];
	int j = 0;
	strcat_s(text, TEXT_SIZE, " ");

	for (int i = 0; text[i]; i++) {
		if (iseng(text[i])) {
			word[j++] = text[i];
		}
		else if (j > 0) {
			word[j] = 0;
			strcrt52(word);
			strrpl(text, word, i - j);

			j = 0;
		}
	}

	// text[strlen(text) - 1] = '\0';

	return text;
}


int main() {
	char text[TEXT_SIZE];
	gets_s(text, TEXT_SIZE);
	f52(text);
	puts(text);
	return 0;
}