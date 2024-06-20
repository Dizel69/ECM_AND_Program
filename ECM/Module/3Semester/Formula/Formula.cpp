// <формула> ::= <цифра> | М(<формула>, <формула>) | m(<формула>, <формула>)
// <цифра> :: = 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9, где М обозначает функцию max, a m - min.
// Вычислить как целое число значение данной формулы
// М(5, m(6,8))=6
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <ctype.h>

const int MAX_STACK_SIZE = 100;
const int MAX_TYPE = 0;
const int MIN_TYPE = 1;

struct STACK
{
	int data[MAX_STACK_SIZE];
	int size;
};

int parseFormul(char* formul);
int execute(const char* filename);
void initStack(STACK* stack);
bool isStackEmpty(STACK* stack);
void push(STACK* stack, int num);
int pop(STACK* stack);

int main()
{
	setlocale(0, "ru");

	int code = execute("Test.txt");
	if (code == 1)
	{
		printf("Файл Test.txt невозможно прочитать\n");
		exit(1);
	}

	printf("Программа завершила работу");
	return 0;
}

int execute(const char* filename)
{
	FILE* file;
	char formul[80];

	if ((file = fopen(filename, "r")) == NULL)
		return 1;

	while (fgets(formul, 80, file) != NULL)
		printf("%s -> %d\n", formul, parseFormul(formul));

	fclose(file);
	return 0;
}

int parseFormul(char* formul)
{
	int len = strlen(formul);
	char buffer[80];
	STACK stack;
	initStack(&stack);

	int i, j;
	for (i = 0; i < len; i++)
	{
		// заталкиваем в стек коды операций и числа
		if (formul[i] == 'M')
		{
			push(&stack, MAX_TYPE);
			continue;
		}
		else if (formul[i] == 'm')
		{
			push(&stack, MIN_TYPE);
			continue;
		}

		if (isdigit(formul[i]))
		{
			j = 0;
			while (isdigit(formul[i]))
				buffer[j++] = formul[i++];
			buffer[j] = ' ';
			push(&stack, atoi(buffer));
		}

		// если попадается закрывающая скобка, вытаскиваем из стека два числа и код
		// операции, производим операцию и закидываем результат обратно в стек
		if (formul[i] == ')')
		{
			int b = pop(&stack);
			int a = pop(&stack);
			int func = pop(&stack);
			if (func == MAX_TYPE)
				push(&stack, a > b ? a : b);
			else
				push(&stack, a < b ? a : b);
		}
	}

	return pop(&stack);
}

void initStack(STACK* stack)
{
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		stack->data[i] = 0;
	stack->size = 0;
}

bool isStackEmpty(STACK* stack)
{
	return stack->size == 0;
}

void push(STACK* stack, int num)
{
	stack->data[stack->size] = num;
	stack->size++;
}

int pop(STACK* stack)
{
	if (isStackEmpty(stack)) return 0;
	else
	{
		stack->size--;
		return stack->data[stack->size];
	}
}
