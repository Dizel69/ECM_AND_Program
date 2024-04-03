/*Создать класс база, имеющий в полях текущую и максимальную размерность. Содержит конструкторы, деструкторы
Создание базы из текстового файла, печать базы, добавление новых записей в базу, удаление записей из базы, корректировка записей в базе
Копирование базы увеличение памяти исходный размер - 100*/
#include <iostream>
#pragma warning (disable:4996)
class employee;


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
	employee(employee& old)
	{
		FIO = new char[strlen(old.FIO)];
		FIO[0] = 0;
		strcpy(FIO, old.FIO);
		years_br[0] = 0;
		strcpy(years_br, old.years_br);
		date_of_admission[0] = 0;
		strcpy(date_of_admission, old.date_of_admission);
		sellary = old.sellary;
	}
	~employee()
	{
		delete[] FIO;
	}
	employee operator +(const employee& old)
	{
		if (this == &old)
		{
			return *this;
		}
		else
		{

			strcpy(years_br, old.years_br);
			strcpy(this->date_of_admission, old.date_of_admission);
			this->sellary = old.sellary;
			delete[] this->FIO;
			this->FIO = new char[strlen(old.FIO)];
			this->FIO[0] = 0;
			strcpy(this->FIO, old.FIO);
			return *this;
		}
	}
	bool findFIO(char* name)
	{
		if (strcmp(FIO, name) == 1)
		{
			return true;
		}
		return false;
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

class db
{
public:
	db();
	db(int n);
	//db(const db* prototype);
	~db();
	void add(employee& new_employee);
	void change_size();
	//private:
	employee* arr;
	int size;
	int max_size;

};

db::db()
{
	max_size = 100;
	arr = new employee[max_size];
	size = 0;
}
db::db(int n)
{
	max_size = n;
	arr = new employee[max_size];
	size = 0;
}
//db::db(const db* prototype)
//{
//	int k = 0;
//	max_size = prototype->max_size;
//	arr = new employee[max_size];
//	size = prototype->size;
//	for (int i = 0; i < size; i++)
//	{
//		if (prototype->arr[i])
//		{
//			arr[k++] = prototype->arr[i];
//		}
//	}
//	size = k+1;
//}

db::~db()
{
	delete[] arr;
}

int createDB(char* fname, db* base);

int main()
{
	//db* base;
	db a(5);
	//base = &a;
	char fname[50] = "database.txt";
	//printf("%d\n", createDB(fname, &a));
	createDB(fname, &a);
	for (int i = 0; i < 5; i++)
	{
		a.arr[i].print();
	}
}

int createDB(char* fname, db* base)
{
	FILE* f;
	employee temp;
	if ((f = fopen(fname, "r")) == NULL)
	{
		return 1;
	}
	char fio[31];
	char born[5];
	float oclad;
	char date[11];
	char inits[5];
	int k = 0;
	while (fscanf(f, "%s%s%s%f%s", fio, inits, born, &oclad, date) == 5)
	{
		strcat(fio, inits);
		employee temp(strlen(fio), oclad, fio, born, date);
		base->add(temp);
		temp.~employee();
	}
	return 0;
}


//Пример добавления записи в базу
void db::add(employee& new_employee)
{
	if (size < max_size)
	{
		arr[size] = new_employee;
		size++;
	}
	else
	{
		this->change_size();
		this->add(new_employee);
	}
}

void db::change_size()
{
	max_size *= 2;
	employee* temp = new employee[max_size];
	for (int i = 0; i < size; i++)
	{
		temp[i] = arr[i];
	}
	delete[] arr;
	arr = temp;
}