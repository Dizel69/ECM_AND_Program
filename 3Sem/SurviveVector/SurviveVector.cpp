#include <string>
#include <iostream>
using namespace std;
#include <vector>
#include "list2.h"
void mass();
void lst();

int main()
{
	setlocale(LC_ALL, "rus");
	int sw;
	cout << "Выберите способ:" << endl << "1. Вектор" << endl << "2. Список" << endl << "3. Выход" << endl;
	cin >> sw;
	while (sw != 3)
	{
		switch (sw)
		{
		case 1:
			mass();
			break;
		case 2:
			lst();
			break;
		}
		cin >> sw;
	}

}

void mass() {
	int n, k, count;//n размерность k константа
	cout << "Введите n и k" << endl;
	cin >> n >> k;
	vector < string > circle(n);
	cout << "Заполните имена участников" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> circle[i];
	}
	count = n;
	for (auto it = circle.begin(); count > 1; )
	{
		if (it >= circle.end())
		{
			it = circle.begin();
		}
		for (int j = 0; j < k - 1; j++)
		{
			it++;
			if (it >= circle.end())
			{
				it = circle.begin();
			}
		}
		cout << *it << endl;
		it = circle.erase(it);
		count--;
	}
	cout << "Полседний участник: " << circle[0] << endl;
}

void lst() {
	int n, k, count;//n размерность k константа
	cout << "Введите n и k" << endl;
	cin >> n >> k;
	list2 list(n);
	list.fill();
	list.game(k);
	cout << "Последний участник: ";
	list.print();
}