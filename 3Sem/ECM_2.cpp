/*
Фио инициалы
год рождения
оклад float
дата поступления
Вывести список сотрудников, стаж работы которых больше 25 лет
*/
#pragma warning(disable:4996)
#include <iostream>
class employee
{
public:
	void set_FIO(char* s);
	void set_years_br(char* br);
	void set_sellary(float k);
	void set_date_of_admission(char* date);
	char* get_FIO();
	char* get_years_br();
	float get_sellary();
	char* get_date_of_admission();
	int get_expirience();
	void print();
	employee()
	{
		sellary = 0.0;
		FIO = new char[31];
		FIO[0] = 0;
		years_br[0] = 0;
		date_of_admission[0] = 0;
	}
	employee(int n)
	{
		sellary = 0.0;
		FIO = new char[n];
		FIO[0] = 0;
		years_br[0] = 0;
		date_of_admission[0] = 0;
	}
	employee(int size, float n, char* f, char* br, char* date)
	{
		sellary = n;
		FIO = new char[size];
		FIO[0] = 0;
		strcpy(FIO, f);
		years_br[0] = 0;
		strcpy(years_br, br);
		date_of_admission[0] = 0;
		strcpy(date_of_admission, date);
	}

private:
	char* FIO;
	char years_br[5];
	float sellary;
	char date_of_admission[11];
};

int employee::get_expirience()
{
	return 2022 - atoi(&date_of_admission[6]);
}

void employee::set_FIO(char* s)
{
	strcpy(FIO, s);
}
void employee::set_years_br(char* br)
{
	strcpy(years_br, br);
}
void employee::set_sellary(float k)
{
	sellary = k;
}
void employee::set_date_of_admission(char* date)
{
	strcpy(date_of_admission, date);
}
char* employee::get_FIO()
{
	return FIO;
}
char* employee::get_years_br()
{
	return years_br;
}
float employee::get_sellary()
{
	return sellary;
}
char* employee::get_date_of_admission()
{
	return date_of_admission;
}
void employee::print()
{
	printf("%s %s %f %s\n", FIO, years_br, sellary, date_of_admission);
}

int main()
{
	setlocale(LC_ALL, "RUS");
	int size, sellary;
	char name[10] = "Saha";
	char br[5] = "2003";
	char date[11] = "05.10.2000";
	printf("Введите количество работников\n");
	scanf_s("%d", &size);
	employee* arr = new employee[size];
	for (int i = 0; i < size; i++)
	{
		printf("Введите Фамилию:\n");
		scanf_s("%s", &name, 10);
		arr[i].set_FIO(name);
		printf("Введите год рождения:\n");
		scanf_s("%s", &br, 5);
		arr[i].set_years_br(br);
		printf("Введите дату начала работы:\n");
		scanf_s("%s", &date, 11);
		arr[i].set_date_of_admission(date);
		printf("Введите оклад:\n");
		scanf_s("%d", &sellary);
		arr[i].set_sellary(sellary);
	}

	for (int i = 0; i < size; i++)
	{
		if (arr[i].get_expirience() > 25)
		{
			arr[i].print();
		}
	}
}