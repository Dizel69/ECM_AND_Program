// 3.2 	На вход программе подается текст на английском языке, в котором слова разделены одним или несколькими пробелами. 
//      Под словом понимается любая последовательность символов, не содержащая пробелов.  Удалить из строки  слова четной длины 
//      и перевернуть слова нечетной длины. Пробелы остаются в  строке без изменения.


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "const.h"

char* strobr(char* str) {
	int len = strlen(str);
	int hlen = len / 2;
	char temp;

	for (int i = 0; i < hlen; i++) {
		temp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = temp;
	}

	return str;
}

char* strdel(char* str, int begin, int count) {
	int len = strlen(str);

	for (int i = 0; begin + count + i <= len; i++) {
		str[begin + i] = str[begin + i + count];
	}

	return str;
}

char* strrpl(char* origin, char* rplmnt, int begin) {
	for (int i = begin, j = 0; rplmnt[j]; i++, j++) {
		origin[i] = rplmnt[j];
	}

	return origin;
}

char* f32(char* text) {
	char word[WORD_SIZE];
	int k = 0;

	strcat(text, " ");
	for (int i = 0; text[i]; i++) {
		if (text[i] != ' ') {
			word[k++] = text[i];
		}
		else if (k > 0) {
			word[k] = 0;

			if (k % 2) {
				strobr(word);
				strrpl(text, word, i - k);
			}
			else {
				strdel(text, i - k, k);
				i -= k;
			}

			k = 0;
		}
	}

	// text[strlen(text) - 1] = '\0';

	return text;
}


int main() {
	char text[TEXT_SIZE];
	gets_s(text, TEXT_SIZE);
	f32(text);
	puts(text);
	return 0;
}
