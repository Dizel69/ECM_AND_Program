#include "Point.h"
#include "Triangle.h"
#include <iostream>
using namespace std;


struct list
{
	Triangle data;
	list* next;
};

list* add_triangle(list* head);
void funcks_menu();
float triangle_area(Point &a, Point &b, Point &c);

int main()
{
	setlocale(LC_ALL, "rus");
	funcks_menu();
}

float triangle_area(Point& a, Point& b, Point& c)
{
	float area = 0.5 * ((a.x_ - c.x_) * (b.y_ - c.y_) - (a.y_ - c.y_) * (b.x_ - c.x_));
	if (area > 0)
	{
		return area;
	}
	return area * (-1);
}

list* add_triangle(list* head)
{
	/*list* cur = head;
	if (!head)
	{
		head = (list*)malloc(sizeof(list));
		head->data.triangle_fill();
		head->next = nullptr;
		return;
	}
	while (cur->next)
	{
		cur = cur->next;
	}
	cur->next = (list*)malloc(sizeof(list));
	cur->next->data.triangle_fill();
	cur->next->next = nullptr;*/
	list* cur, * pr, * pnew;
	pnew = new list;
	pnew->data = *new Triangle;
	pnew->data.triangle_fill();
	pnew->next = NULL;
	if (head == NULL)
	{
		return pnew;
	}
	pr = NULL;
	cur = head;
	while (cur->next != NULL)
	{
		pr = cur;
		cur = pr->next;
	}
	cur->next = pnew;
	return head;
}

void funcks_menu()
{
	list* head = nullptr;
	list* cur,*cur1;
	Point temp_point;
	printf("1 Ввести треугольник\n");
	printf("2 Напечатать площадь\n");
	printf("3 Переместить на плоскости\n");
	printf("4 Сравнить треугольники\n");
	printf("5 Проверить на вхождение\n");
	printf("6 Exit\n");
	int variant,temp,temp1,count=0;

	do {
		scanf_s("%d", &variant);
		switch (variant)
		{
		case 1:
		{
			head = add_triangle(head);
			break;
		}
		case 2:
		{
			cur = head;
			printf("Введите номер треугольника\n");
			scanf_s("%d", &temp);
			for (int i = 1; i <temp ; i++)
			{
				cur = cur->next;
			}
			printf("%.1f\n", cur->data.GetArea());
			break;
		}
		case 3:
		{
			cur = head;
			printf("Введите номер треугольника\n");
			scanf_s("%d", &temp);
			for (int i = 1; i < temp; i++)
			{
				cur = cur->next;
			}
			printf("На сколько сдвинуть по х?\n");
			scanf_s("%f", &temp_point.x_);
			printf("На сколько сдвинуть по y?\n");
			scanf_s("%f", &temp_point.y_);
			cur->data.move(temp_point);
			break;
		}
		case 4:
		{
			cur = head;
			cur1 = head;
			printf("Введите номера треугольников\n");
			scanf_s("%d%d", &temp,&temp1);
			for (int i = 1; i < temp; i++)
			{
				cur = cur->next;
			}
			for (int i = 1; i < temp1; i++)
			{
				cur1 = cur1->next;
			}
			if (cur->data>cur1->data)
			{
				printf("Первый треугольник больше\n");
			}
			if (cur->data < cur1->data)
			{
				printf("Первый треугольник меньше\n");
			}
			if (cur->data == cur1->data)
			{
				printf("Треугольники равны\n");
			}
			break;
		}
		case 5:
		{
			cur = head;
			cur1 = head;
			printf("Введите номера треугольников\n");
			scanf_s("%d%d", &temp, &temp1);
			for (int i = 1; i < temp1; i++)
			{
				cur = cur->next;
			}
			for (int i = 1; i < temp; i++)
			{
				cur1 = cur1->next;
			}
			if (cur->data.check_include(cur1->data))
			{
				printf("Треугольник %d входит в треугольник %d\n",temp,temp1);
			}
			else
			{
				printf("Треугольник %d не входит в треугольник %d\n",temp,temp1);
			}
			break;
		}

		}
	} while (variant != 6);
}