// 2.2 На вход программы подается текст на английском языке. Требуется
//     написать программу, которая определяет и выводит на экран английскую
//     букву, встречающуюся в этом тексте чаще всего, и количество таких
//     букв.Строчные и прописные буквы при этом считаются не
//     различимыми.Если искомых букв несколько, то программа должна
//     выводить на экран первую из них по алфавиту.

#include <iostream>
#include <ctype.h>

const int STR_LENGTH = 80;

int main()
{
	char str[STR_LENGTH] = "AJfakfnasoa aj falKNF alkf nasflk asnf";
	char tempChar;
	int maxIndex = 0;
	int alphabet[26] = { 0 };

	for (int i = 0; str[i] != '\0'; i++)
	{
		tempChar = tolower(str[i]);

		if (tempChar >= 'a' && tempChar <= 'z')
			alphabet[tempChar - 'a']++;
	}

	for (int i = 0; i < 26; i++)
	{
		if (alphabet[i] > alphabet[maxIndex])
		{
			maxIndex = i;
		}
	}

	printf("%c %d", 'A' + maxIndex, alphabet[maxIndex]);

	return 0;
} 
