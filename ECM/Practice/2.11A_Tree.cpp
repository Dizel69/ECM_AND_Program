#include <iostream>
using namespace std;

int tabs = 0; //Для создания отступов
int kol_vo = 0;
//Кол-во отступов высчитывается по кол-ву рекурсивного вхождения при выводе в фукцию print

//Структура ветки
struct Tree
{
	int Data; //Поле данных
	Tree* LeftBranch; //УКАЗАТЕЛИ на соседние веточки
	Tree* RightBranch;
};


//Функция внесения данных
void Add(int aData, Tree*& aBranch)
{
	//Если ветки не существует
	if (!aBranch)
	{ //создадим ее и зададим в нее данные
		aBranch = new Tree;
		aBranch->Data = aData;
		aBranch->LeftBranch = 0;
		aBranch->RightBranch = 0;
		return;
	}
	else //Иначе сверим вносимое
		if (aBranch->Data > aData)
		{ //Если оно меньше того, что в этой ветке - добавим вправо
			Add(aData, aBranch->RightBranch);
		}
		else
		{ //Иначе в ветку слева
			Add(aData, aBranch->LeftBranch);
		};
}

//Функция вывода дерева
void print(Tree* aBranch)
{
	if (!aBranch) return; //Если ветки не существует - выходим. Выводить нечего
	tabs += 5; //Иначе увеличим счетчик вызванных процедур
	//Который будет считать нам отступы для красивого вывода

	print(aBranch->LeftBranch); //Выведем ветку и ее подветки слева

	for (int i = 0; i < tabs; i++) cout << " "; //Потом отступы
	cout << aBranch->Data << endl; //Данные этой ветки


	print(aBranch->RightBranch);//И ветки, что справа

	tabs -= 5; //После уменьшим кол-во отступов
	return;
}

void FreeTree(Tree* aBranch)
{
	if (!aBranch) return;
	FreeTree(aBranch->LeftBranch);
	FreeTree(aBranch->RightBranch);
	delete aBranch;
	return;
}

Tree* del_elem(Tree*& aBranch, int aData) {
	if (aBranch == NULL)
		return aBranch;

	if (aData == aBranch->Data) {

		Tree* tmp;
		if (aBranch->RightBranch == NULL)
			tmp = aBranch->LeftBranch;
		else {

			Tree* ptr = aBranch->RightBranch;
			if (ptr->LeftBranch == NULL) {
				ptr->LeftBranch = aBranch->LeftBranch;
				tmp = ptr;
			}
			else {

				Tree* pmin = ptr->LeftBranch;
				while (pmin->LeftBranch != NULL) {
					ptr = pmin;
					pmin = ptr->LeftBranch;
				}
				ptr->LeftBranch = pmin->RightBranch;
				pmin->LeftBranch = aBranch->LeftBranch;
				pmin->RightBranch = aBranch->RightBranch;
				tmp = pmin;
			}
		}

		delete aBranch;
		return tmp;
	}
	else if (aData < aBranch->Data)
		aBranch->LeftBranch = del_elem(aBranch->LeftBranch, aData);
	else
		aBranch->RightBranch = del_elem(aBranch->RightBranch, aData);
	return aBranch;
}

int main()
{
	setlocale(LC_ALL, "rus");
	Tree* Root = 0;
	int vel = 0;
	int element;
	int num = 0;

	printf("Введите количество элементов\n");
	scanf_s("%d", &vel);

	for (int i = 0; i < vel; i++)
	{
		printf("Введите число:\n");
		scanf_s("%d", &num);
		Add(num % 100, Root);
	}

	printf("Вывод бинарного дерева:\n ");
	print(Root);

	printf("Удалим элемент из бинарного дерева:");
	printf("Введите элемент: ");
	scanf_s("%d", &element);
	del_elem(Root, element);

	printf("Вывод бинарного дерева: ");
	print(Root);

	return 0;
}