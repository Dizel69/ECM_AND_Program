// 1.11 Получить сумму двух целых чисел. Числа представлены в виде двух стеков. 
//		Результат представлен линейным односвязанным списком, информационная часть элементов которого
//		такая же, как у элементов стека.


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

const int STACK_SIZE = 30;

struct DATA
{
    char* sign;
    int* amount;
    int digit;
};

struct STACK
{
    DATA info[STACK_SIZE];
    int size;
};

struct NODE
{
    DATA info;
    NODE* next;
};

bool push(STACK* stack, DATA data);
DATA pop(STACK* stack);
bool fillStack(STACK* stack, int* amount);
int math(STACK* stack1, STACK* stack2, int* amount1, int* amount2);
NODE* fillLinSp(NODE* head, int* amount, int el);
NODE* addToEnd(NODE** head, DATA el);
void printList(NODE* head);

int main()
{
    setlocale(LC_ALL, "ru");
    STACK stack1, stack2;
    DATA head1, head2;
    NODE* list = NULL;
    stack1.size = 0;
    stack2.size = 0;

    int head1Amount = 0;
    int head2Amount = 0;
    int listAmount = 0;
    bool fl = false;

    printf("Введите первое число: ");
    if (!fillStack(&stack1, &head1Amount))
    {
        printf("Вы не ввели первое число!\n");
        exit(1);
    }

    printf("Введите второе число: ");
    if (!fillStack(&stack2, &head2Amount))
    {
        printf("Вы не ввели второе число!\n");
        exit(1);
    }

    int res = math(&stack1, &stack2, &head1Amount, &head2Amount);

    list = fillLinSp(list, &listAmount, res);
    printList(list);

    return 0;
}

int reverse(int n)
{
    int result = 0;

    while (n > 0)
    {
        result = result * 10 + n % 10;
        n /= 10;
    }

    return result;
}

int math(STACK* stack1, STACK* stack2, int* amount1, int* amount2)
{
    int num1 = 0, num2 = 0, mod = 1;
    int rez = 0;

    while (stack1->size > 1)
    {
        num1 += pop(stack1).digit * mod;
        mod *= 10;
    }
    if (pop(stack1).sign[0] == '-')
        num1 = -num1;

    mod = 1;
    while (stack2->size > 1)
    {
        num2 += pop(stack2).digit * mod;
        mod *= 10;
    }
    if (pop(stack2).sign[0] == '-')
        num2 = -num2;

    rez = num1 + num2;

    printf("%d %d %d\n", num1, num2, rez);
    return rez;
}

bool fillStack(STACK* stack, int* amount)
{
    char sign[] = "+";
    char c;
    bool fl = false;
    DATA data, head;
    head.amount = amount;
    head.digit = NULL;

    data.amount = NULL;
    data.sign = NULL;

    scanf("%c", &c);
    if (c == '-')
    {
        sign[0] = '-';
        scanf("%c", &c);
    }
    else if (c == '+')
        scanf("%c", &c);

    head.sign = new char[2];
    strcpy(head.sign, sign);
    printf("%s\n", sign);
    push(stack, head);

    while (c != '\n')
    {
        fl = true;
        data.digit = c - '0';
        if (push(stack, data))
            *amount += 1;
        scanf("%c", &c);
    }

    return fl;
}

bool push(STACK* stack, DATA data)
{
    if (stack->size >= STACK_SIZE)
        return false;

    stack->info[stack->size] = data;
    stack->size++;

    return true;
}

DATA pop(STACK* stack)
{
    if (stack->size == 0)
    {
        DATA temp;
        temp.amount = NULL;
        temp.digit = NULL;
        temp.sign = NULL;

        return temp;
    }

    stack->size--;
    return stack->info[stack->size];
}

NODE* fillLinSp(NODE* head, int* amount, int el)
{
    char sign[2];
    sign[1] = 0;
    int ost;
    DATA hd;
    hd.sign = new char[2];
    hd.amount = amount;
    hd.digit = NULL;

    if (el < 0)
        sign[0] = '-';
    else
        sign[0] = '+';
    strcpy(hd.sign, sign);
    printf("%s\n", sign);
    head = addToEnd(&head, hd);

    if (el < 0)
        el = -el;

    while (el != 0)
    {
        ost = el % 10;

        DATA data;
        data.amount = NULL;
        data.sign = NULL;
        data.digit = ost;
        head = addToEnd(&head, data);
        *amount += 1;

        el /= 10;
    }

    return head;
}

NODE* addToEnd(NODE** head, DATA el)
{
    NODE* newPart = new NODE;
    newPart->info = el;
    newPart->next = NULL;

    if (*head == NULL)
        *head = newPart;
    else
    {
        NODE* curr = *head;

        while (curr->next != NULL)
            curr = curr->next;

        curr->next = newPart;
    }

    return *head;
}

void printList(NODE* head)
{
    printf("Головка: \n");
    printf("Знак: %s\nКоличество цифр: %d\n", head->info.sign, *head->info.amount, head->next);
    NODE* curr = head->next;

    while (curr != NULL)
    {
        printf("%d -> %p\n", curr->info.digit, curr->next);
        curr = curr->next;
    }
}
