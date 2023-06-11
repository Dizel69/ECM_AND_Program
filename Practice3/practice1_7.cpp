// Написать программу «Моя записная книжка».
//Предусмотреть возможность работы с произвольным числом записей, поиска записи по какому - либо признаку
//(например, по фамилии, дате рождения или номеру телефона), добавления и удаления записей, сортировки по разным полям.
//Хранение данных организовать с применением контейнерного класса.


#include<iostream>
#include<ostream>
#include<string>
#include<list>
#include<Windows.h>

using namespace std;
class Аbonent
{
protected:
    string fname;
    string lname;
    string PhoneNumber;
    string DateofBirth;
public:
    Аbonent()
    {
        fname = "NULL";
        lname = "NULL";
        PhoneNumber = "NULL";
        DateofBirth = "NULL";
    }
    Аbonent(string f, string l, string e, string P, string D)
    {
        fname = f;
        lname = l;
        PhoneNumber = P;
        DateofBirth = D;
    }
    void Setlname()
    {
        cout << "введите фамилию" << endl;
        cin >> lname;
    }
    string Getlname()
    {
        return this->lname;
    }

    void Setfname()
    {
        cout << "введите имя" << endl;
        cin >> fname;
    }
    string Getfname()
    {
        return this->fname;
    }
    void SetPhone()
    {
        cout << "введите номер телефона " << endl;
        cin >> PhoneNumber;
    }
    string GetPhone()
    {
        return PhoneNumber;
    }
    void SetDateofbirth()
    {
        cout << "введите дату рождения" << endl;
        cin >> DateofBirth;
    }
    string GetDateofBirth()
    {
        return this->DateofBirth;
    }
    void setallINFO()
    {
        Setlname();
        Setfname();
        SetPhone();
        SetDateofbirth();
    }
    friend ostream& operator<< (ostream& out, Аbonent& obj);
    friend istream& operator>>(istream& in, Аbonent& obj);
};
std::ostream& operator<< (std::ostream& out, Аbonent& obj)
{
    out << obj.lname << endl;
    out << obj.fname << endl;
    out << obj.PhoneNumber << endl;
    out << obj.DateofBirth << endl;
    return out;
}
std::istream& operator>>(std::istream& in, Аbonent& obj)
{
    in >> obj.lname;
    in >> obj.DateofBirth;
    in >> obj.fname;
    in >> obj.PhoneNumber;
    return in;
}
bool Sr(Аbonent a1, Аbonent a2)//предикат для метода сортировки
{
    return a1.Getlname() > a2.Getlname();
}
class Notebook
{
private:
    list<Аbonent>Note;

public:
    string lname = "NULL";
    void add_client()
    {
        /* Добавление новой записи*/
        Аbonent* tmp = new Аbonent();
        tmp->setallINFO();
        this->Note.push_back(*tmp);
        delete tmp;
    }
    void add_client(Аbonent tmp) /* перегрузка метода добавления новой записи*/
    {
        this->Note.push_back(tmp);
    }
    /*  вывод всех записей*/
    void Output()
    {
        list<Аbonent>::iterator IT = Note.begin();
        for (; IT != Note.end(); IT++)
        {
            cout << *IT << endl;
        }
    }
    void SearchByFname(string F)//поиск по имени
    {
        bool f = false;
        list<Аbonent>::iterator IT = Note.begin();
        for (; IT != Note.end(); IT++)
        {
            if (F == (*IT).Getfname())
                cout << *IT << endl;
            f = true;
        }
        if (!f)
            cout << "нет такого имени в записной книжке" << endl;
    }
    void SearchByLname(string L)//поиск по фамилии
    {
        bool l = false;
        list<Аbonent>::iterator IT = Note.begin();
        for (; IT != Note.end(); IT++)
        {
            if (L == (*IT).Getlname())
                cout << *IT << endl;
            l = true;
        }
        if (!l)
            cout << "нет такой фамилии в записной книжке" << endl;
    }
    void SearchByPhoneNumber(string P)//поиск по номеру телефона
    {
        bool p = false;
        list<Аbonent>::iterator IT = Note.begin();
        for (; IT != Note.end(); IT++)
        {
            if (P == (*IT).GetPhone())
                cout << *IT << endl;
            p = true;
        }
        if (!p)
            cout << "нет такого номера телефона в записной книжке" << endl;
    }
    void SearchByDateofBirth(string D)//поиск по дате рождения
    {
        bool d = false;
        list<Аbonent>::iterator IT = Note.begin();
        for (; IT != Note.end(); IT++)
        {
            if (D == (*IT).GetDateofBirth())
                cout << *IT << endl;
            d = true;
        }
        if (!d)
            cout << "нет такой даты в записной книжке" << endl;
    }
    void SortBylname()//сортировка по фамилии
    {

        Note.sort(Sr);

    }
    void searchinfo(string value)//поиск по нескольким признакам
    {
        list<Аbonent>::iterator iter = Note.begin();
        for (; iter != Note.end(); iter++)
        {
            if ((*iter).GetDateofBirth() == value ||
                ((*iter).Getfname() == value) ||
                ((*iter).Getlname() == value) ||
                ((*iter).GetPhone() == value))
            {
                cout << *iter << endl;
            }
        }
    };
    void deleteinfo(string value) //удаление записи
    {
        list<Аbonent>::iterator iter = Note.begin();
        for (; iter != Note.end(); iter++)
        {
            if ((*iter).GetDateofBirth() == value ||
                ((*iter).Getfname() == value) ||
                ((*iter).Getlname() == value) ||
                ((*iter).GetPhone() == value))
            {
                cout << "Найдена запись" << endl;
                cout << *iter << endl;
                cout << "Удалить?(1 - да, 0 - нет)" << endl;
                int a;
                cin >> a;
                if (a != 0)
                    Note.erase(iter);
                cout << "запись удалена" << endl;
                break;
            }
        }
    }
};
void Menu_()
{
    cout << "Заполните пожалуйста данные" << endl;
    cout << "1 - Добавить новую запись" << endl;
    cout << "2 - Показать информацию по имени" << endl;
    cout << "3 - Показать информацию по фамилии" << endl;
    cout << "4 - Показать информацию по Дате рождения" << endl;
    cout << "5 - Показать информацию по телефону" << endl;
    cout << "6 - Сортировать по фамилии" << endl;
    cout << "7 - Показать информацию по нескольким признакам" << endl;
    cout << "8 - Удалить информацию" << endl;
    cout << "9 - EXIT" << endl;
}
int main()
{
    system("chcp 1251 > nul");
   // SetConsoleCp(1251);
    //SetConsoleOutput(1251);
    Аbonent A;
    Notebook N;
    string TMP;
    list<Аbonent>::iterator IT;
    int Menu = 0;
    int param;
    do
    {
        system("cls");
        Menu_();
        cin >> Menu;
        switch (Menu)
        {
        case 1:
            cout << "добавление новой записи" << endl;
            N.add_client();
            break;
        case 2:
            cout << "Показ информации по имени" << endl;
            cout << "Введите имя" << endl;
            cin >> TMP;
            N.SearchByFname(TMP);
            cout << "Введите другое имя" << endl;
            break;
        case 3:
            cout << "Показ информации по фамилии" << endl;
            cout << "Введите фамилию" << endl;
            cin >> TMP;
            N.SearchByLname(TMP);
            cout << "Введите другую фамилию" << endl;
            break;
        case 4:
            cout << "Показ информации по Дате Рождения" << endl;
            cout << "Введите Дату Рождения" << endl;
            cin >> TMP;
            N.SearchByDateofBirth(TMP);
            cout << "Введите другую дату рождения" << endl;
            break;
 
        case 5:
            cout << "Показ информации по номеру телефона" << endl;
            cout << "Введите номер телефона" << endl;
            cin >> TMP;
            N.SearchByPhoneNumber(TMP);
            cout << "Введите другой номер телефона" << endl;
            break;
        case 6:
            cout << "Сортировка по фамилии" << endl;
            N.SortBylname();
            N.Output();
            break;
        case 7:
            cin >> TMP;
            N.searchinfo(TMP);
            cout << "Поиск информации завершен" << endl;
            break;
        case 8:
            cin >> TMP;
            N.deleteinfo(TMP);
            break;
        }
        system("pause");
    } while (Menu != 9);
    system("pause");
    return 0;
}