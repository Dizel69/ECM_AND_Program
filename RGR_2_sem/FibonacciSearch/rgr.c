//Поиск слова в файле, с помощью поиска Фибоначчи

#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
int min(int a, int b);
int fibSearch(char* filneme, int n, char* x);
int comparision_int(int a,int b);
int comparison_char(char* first,char* second);

int main(int argc,char* argv[])
{
	if (argc != 3)
	{
		printf("Не верное число аргументов\n");
		exit(1);
	}
	
//  int size;
//	char file[10]="dick",word[50]="c";
	printf("%d\n",fibSearch(argv[1],9	,argv[2]));
	//char word1[50]="zzzz";
	//printf("%d\n",comparison_char(word,word1));
}

int fibSearch(char* filename, int n, char* x)
{
	FILE* f;
	char word[50];
	if ((f=fopen(filename,"r"))==NULL)
	{
		//printf("xyu\n");
		return -1;
	}
	
	int start = -1, i;

	int f0 = 0, f1 = 1, f2 = 1;
	while (f2 < n) {
		f0 = f1; f1 = f2; f2 = f0 + f1;
	}
	//printf("%d %d %d\n",f0,f1,f2);
	while (f2 > 1) {
		i = min(start + f0, n - 1);
		fseek(f,0L, SEEK_SET);
		for (int j = 0; j < i+1; j++)
		{
			fscanf(f,"%s",word);
		//	printf("%s",word);
		}
		//printf("%s\n",word);
		if (comparison_char(word,x)==0) {
			f2 = f1; f1 = f0; f0 = f2 - f1;

			start = i;
		}
		else if (comparison_char(word,x)==1) {
			f2 = f0; f1 = f1 - f0; f0 = f2 - f1;
		}
		else {
			return i+1;
		}
	}

	if (f1 && comparison_char(word,x)==2) {

		return n;
	}

	return -2;
}	

int min(int a, int b)
{
	return a < b ? a : b;
}
int comparision_int(int a,int b)
{
	return a > b ? a : b;
}
int comparison_char(char* first,char* second)
{
	int len = min(strlen(first),strlen(second));
	for (int i = 0; i < len; i++)
	{
		if (first[i]<second[i])
		{
			return 0;
		}
		else if (first[i]>second[i])
		{
			return 1;
		}				
	}
	if (strlen(first)==strlen(second))
	{
		return 2;
	}
	else
	{
		if (strlen(first)>strlen(second))
		{
			return 1;
		}
		else
			return 0;
		
	}
	
	
	
	
}