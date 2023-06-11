#include <stdio.h>
#include <string.h>
#include <limits>
#include "const.h"

int strlen_a(char* str, int strSize) {
	int len = 0;
	while (*str++) len++;

	return len;
}

char* strcat_a(char* part1, int part1Size, char* part2, int part2Size) {
	if (strlen_a(part1, part1Size) + strlen_a(part2, part2Size) >= part1Size) {
		return NULL;
	}

	char* p_beg = part1;

	while (*part1) part1++;
	while (*part1++ = *part2++);
	*part1 = '\0';

	return p_beg;
}

char* strcpy_a(char* _copy, int copySize, char* origin, int originSize) {
	if (strlen_a(origin, originSize) >= copySize) return NULL;

	char* p_beg = _copy;

	while (*_copy++ = *origin++);
	*_copy = '\0';

	return p_beg;
}

char* strchr_a(char* str, int strSize, char chr) {
	while (*str) {
		if (*str == chr) return str;
		str++;
	}

	return NULL;
}

char* strstr_a(char* str, int strSize, char* substr, int subSize) {
	int len = strlen_a(str, strSize);
	int sublen = strlen_a(substr, subSize);

	for (int i = 0, j = 0; i < len; i++) {
		if (str[i] == substr[j]) {
			j++;
		}
		else {
			j = 0;
		}

		if (j == sublen) return &str[i - j + 1];
		if (!j && i + sublen > len) break;
	}

	return NULL;
}

int strcmp_a(char* str1, int str1Size, char* str2, int str2Size) {
	while (*str1 && *str2 && *str1 == *str2) {
		str1++; str2++;
	}

	return *str1 - *str2;
}

int max(int* arr, int n, int& index) {
	int max_value = INT_MIN;

	for (int i = 0; i < n; i++) {
		if (arr[i] > max_value) {
			max_value = arr[i];
			index = i;
		}
	}

	return max_value;
}

int* f22(char* text) {
	int* number_of_letter = new int[26];
	for (int i = 0; i < 26; i++) {
		number_of_letter[i] = 0;
	}

	while (*text) {
		if (*text >= 'a' && *text <= 'z') number_of_letter[*text - 'a']++;
		if (*text >= 'A' && *text <= 'Z') number_of_letter[*text - 'A']++;

		text++;
	}

	return number_of_letter;
}

int* f23(char* text) {
	int* number_of_digit = new int[10];
	for (int i = 0; i < 10; i++) {
		number_of_digit[i] = 0;
	}

	while (*text) {
		if (*text >= '0' && *text <= '9') number_of_digit[*text - '0']++;
		text++;
	}

	return number_of_digit;
}

void joinDigitsMax(int* digits) {
	int i;
	for (i = 0; i < 10; i++) {
		if (digits[i]) {
			printf("Yes\n");
			break;
		}
	}
	if (i == 10) {
		printf("No\n");
		return;
	}

	for (i = 9; i >= 0; i--) {
		for (int j = 0; j < digits[i]; j++) {
			printf("%d", i);
		}
	}
	printf("\n");
}


int main() {
	char text[STRING_SIZE];
	gets_s(text, STRING_SIZE);
	int* number_of_digits = f23(text);
	joinDigitsMax(number_of_digits);
	delete[] number_of_digits;
	return 0;
}
