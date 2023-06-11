#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>

using namespace std;

struct Node
{
    int data1;
    int data2;
    Node* next;
};

class BoolMatrix
{
private:
    Node* head;
    int maxR;
    int maxC;
public:
    BoolMatrix()
    {
        head = NULL;
        maxR = 0;
        maxC = 0;
    }
    BoolMatrix(const BoolMatrix& other)
    {
        head = NULL;
        Node* otherTemp = other.head;
        while (otherTemp != NULL)
        {
            add(otherTemp->data1, otherTemp->data2);
            otherTemp = otherTemp->next;
        }
        maxR = other.maxR;
        maxC = other.maxC;
    }
    ~BoolMatrix()
    {
        Node* temp = head;
        while (temp != NULL)
        {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
        head = NULL;
    }
    void add(int value1, int value2) {
        Node* node = new Node;
        node->data1 = value1;
        node->data2 = value2;
        node->next = NULL;

        if (head == NULL)
            head = node;
        else
        {
            Node* temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = node;
        }
    }
    bool get(int row, int col) const
    {
        Node* temp = head;
        while (temp != NULL)
        {
            if (temp->data1 == row && temp->data2 == col)
                return true;
            temp = temp->next;
        }
        return false;
    }
    BoolMatrix inversion() {

        int maxRow = -1;
        int maxCol = -1;
        Node* node = head;
        while (node != NULL) {
            if (node->data1 > maxRow)
                maxRow = node->data1;
            if (node->data2 > maxCol)
                maxCol = node->data2;
            node = node->next;
        }
        maxRow = maxR;
        maxCol = maxC;
        maxRow--;
        maxCol--;


        BoolMatrix invertedMatrix;
        invertedMatrix.maxR = maxRow;
        invertedMatrix.maxC = maxCol;


        for (int row = 0; row <= maxRow; row++) {
            for (int col = 0; col <= maxCol; col++) {
                bool isOne = false;


                Node* node = head;
                while (node != NULL) {
                    if (node->data1 == row && node->data2 == col) {
                        isOne = true;
                        break;
                    }
                    node = node->next;
                }


                if (isOne)
                    continue;
                else
                    invertedMatrix.add(row, col);
            }
        }
        invertedMatrix.maxR++;
        invertedMatrix.maxC++;
        *this = invertedMatrix;


        return *this;
    }
    BoolMatrix submatrix(int rowStart, int rowEnd, int colStart, int colEnd)
    {
        BoolMatrix result;
        Node* temp = head;
        while (temp != NULL)
        {
            if (temp->data1 >= rowStart && temp->data1 <= rowEnd && temp->data2 >= colStart && temp->data2 <= colEnd)
                result.add(temp->data1 - rowStart, temp->data2 - colStart);
            temp = temp->next;
        }
        *this = result;
        maxC = colEnd - colStart;
        maxR = rowEnd - rowStart;
        maxR++;
        maxC++;
        return *this;
    }
    BoolMatrix transpose()
    {
        BoolMatrix transposedMatrix;
        Node* temp = head;
        while (temp != NULL)
        {
            transposedMatrix.add(temp->data2, temp->data1);
            temp = temp->next;
        }
        transposedMatrix.maxC = maxC;
        transposedMatrix.maxR = maxR;
        *this = transposedMatrix;
        maxR = transposedMatrix.maxC;
        maxC = transposedMatrix.maxC;
        return *this;
    }
    BoolMatrix iteration()
    {
        BoolMatrix temp = *this;
        int maxr = maxR;
        int maxc = maxC;
        BoolMatrix res;
        res.maxR = maxR;
        res.maxC = maxC;

        BoolMatrix one;
        one.maxR = maxR;
        one.maxC = maxC;
        for (size_t i = 0; i < maxr; i++)
        {
            for (size_t j = 0; j < maxr; j++)
            {
                if (i == j)
                    one.add(i, j);

            }
        }
        BoolMatrix a;
        a.maxC = maxC;
        a.maxR = maxR;
        res = one | temp;
        while (res != a)
        {
            temp = temp * temp;
            a = res;
            res = res | temp;

        }
        maxR = res.maxR;
        maxC = res.maxC;
        *this = res;

        return *this;
    }
    BoolMatrix& operator=(const BoolMatrix& other)
    {
        if (this == &other)
            return *this;



        Node* temp = head;
        while (temp != NULL)
        {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
        head = NULL;


        Node* otherTemp = other.head;
        while (otherTemp != NULL)
        {
            add(otherTemp->data1, otherTemp->data2);
            otherTemp = otherTemp->next;
        }
        maxR = other.maxR;
        maxC = other.maxC;


        return *this;
    }
    BoolMatrix operator&(const BoolMatrix& other) const
    {
        BoolMatrix result;
        Node* temp1 = head;
        Node* temp2 = other.head;

        while (temp1 != NULL && temp2 != NULL)
        {
            if (temp1->data1 == temp2->data1 && temp1->data2 == temp2->data2)
            {
                result.add(temp1->data1, temp1->data2);
                temp1 = temp1->next;
                temp2 = temp2->next;
            }
            else if (temp1->data1 < temp2->data1 || (temp1->data1 == temp2->data1 && temp1->data2 < temp2->data2))
                temp1 = temp1->next;

            else
                temp2 = temp2->next;

        }
        result.maxC = maxC;
        result.maxR = maxR;
        return result;
    }
    BoolMatrix operator|(const BoolMatrix& other) const {
        BoolMatrix result(*this);

        Node* temp = other.head;
        while (temp != NULL) {
            bool found = false;
            Node* resultTemp = result.head;
            while (resultTemp != NULL) {
                if (resultTemp->data1 == temp->data1 && resultTemp->data2 == temp->data2) {
                    found = true;
                    break;
                }
                resultTemp = resultTemp->next;
            }
            if (!found)
                result.add(temp->data1, temp->data2);

            temp = temp->next;
        }

        result.maxC = maxC;
        result.maxR = maxR;
        return result;
    }
    BoolMatrix operator*(const BoolMatrix& other)
    {
        BoolMatrix res;
        Node* temp = head;
        Node* temp2 = other.head;
        int rows = 0, cols = 0;
        int rows2 = 0, cols2 = 0;

        while (temp != NULL)
        {
            rows = max(rows, temp->data1);
            cols = max(cols, temp->data2);

            temp = temp->next;
        }
        rows++;
        cols++;
        while (temp2 != NULL)
        {
            rows2 = max(rows2, temp2->data1);
            cols2 = max(cols2, temp2->data2);

            temp2 = temp2->next;
        }
        rows2++;
        cols2++;

        int** arr = new int* [rows];
        for (int i = 0; i < rows; i++)
        {
            arr[i] = new int[cols];
            for (int j = 0; j < cols; j++)
                arr[i][j] = 0;
        }

        int** arr2 = new int* [rows2];
        for (int i = 0; i < rows2; i++)
        {
            arr2[i] = new int[cols2];
            for (int j = 0; j < cols2; j++)
                arr2[i][j] = 0;

        }
        temp = head;
        while (temp != NULL)
        {
            arr[temp->data1][temp->data2] = 1;
            temp = temp->next;
        }
        temp2 = other.head;
        while (temp2 != NULL)
        {
            arr2[temp2->data1][temp2->data2] = 1;
            temp2 = temp2->next;
        }

        int** result = new int* [rows];
        for (int i = 0; i < rows; i++)
        {
            result[i] = new int[cols];
        }
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols2; j++)
            {
                result[i][j] = false;
                for (int k = 0; k < cols; k++)
                    result[i][j] = result[i][j] || (arr[i][k] && arr2[k][j]);

            }
        }

        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols2; j++)
            {
                if (result[i][j] == 1)
                    res.add(i, j);

            }
        }

        for (int i = 0; i < rows; i++)
            delete[] arr[i];
        delete[] arr;

        for (int i = 0; i < rows2; i++)
            delete[] arr2[i];

        delete[] arr2;

        res.maxC = maxC;
        res.maxR = maxR;
        return res;
    }
    bool operator==(const BoolMatrix& b) {
        Node* tempA = head;
        Node* tempB = b.head;
        while (tempA != NULL && tempB != NULL) {
            if (tempA->data1 != tempB->data1 || tempA->data2 != tempB->data2)
                return false;
            tempA = tempA->next;
            tempB = tempB->next;
        }
        return tempA == NULL && tempB == NULL;
    }
    bool operator!=(const BoolMatrix& b) {
        return !(*this == b);
    }
    friend istream& operator>>(istream& is, BoolMatrix& matrix)
    {

        Node* temp = matrix.head;
        while (temp != NULL)
        {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
        matrix.head = NULL;


        char ch;
        is >> ch;
        if (ch != '[')
        {
            cout << "error input";
            exit(0);
        }

        int row = 0, col = 0,max_col=0;//row - строка
        while (true)
        {
            is >> ch;
            if (ch == ';')
            {
                row++;
                col = 0;
            }
            else if (ch == ']')
            {
                break;
            }
            else if (ch == ' ')
            {

            }
            else if (ch == '0')
            {
                col++;
                max_col = max(max_col, col);
            }
            else if (ch == '1')
            {
                matrix.add(row, col);
                col++;
                max_col = max(max_col, col);
            }
            else
            {
                cout << "error input";
                exit(0);
            }
        }
        row++;

        matrix.maxR = row;
        matrix.maxC = max_col;
     //   cout << row << max_col << endl;
        return is;
    }
    friend ostream& operator<<(ostream& os, const BoolMatrix& matrix)
    {
        if (matrix.head == NULL)
        {
            os << "[]";
            return os;
        }


        int maxRow = matrix.head->data1;
        int maxCol = matrix.head->data2;
        Node* temp = matrix.head;
        while (temp != NULL)
        {
            if (temp->data1 > maxRow)
            {
                maxRow = temp->data1;
            }
            if (temp->data2 > maxCol)
            {
                maxCol = temp->data2;
            }
            temp = temp->next;
        }
        maxRow = matrix.maxR;
        maxCol = matrix.maxC;
        maxCol--;
        maxRow--;

        os << "[";
        for (int row = 0; row <= maxRow; row++)
        {
            if (row != 0)
            {
                os << ";";
            }
            for (int col = 0; col <= maxCol; col++)
            {
                bool found = false;
                temp = matrix.head;
                while (temp != NULL)
                {
                    if (temp->data1 == row && temp->data2 == col)
                    {
                        found = true;
                        break;
                    }
                    temp = temp->next;
                }
                if (found)
                {
                    os << (col == 0 ? "" : " ") << "1";
                }
                else
                {
                    os << (col == 0 ? "" : " ") << "0";
                }

            }
        }
        os << "]";

        return os;
    }

    class Iterator {
    private:
        Node* ptr;
    public:
        Iterator(Node* p = nullptr) : ptr(p) {}

        Iterator& operator++()
        {
            ptr = ptr->next;
            return *this;
        }

        int operator*() const
        {
            return ptr->data1 * ptr->data2;
        }

        bool operator!=(const Iterator& other) const
        {
            return ptr != other.ptr;
        }
    };
    Iterator begin() const
    {
        return Iterator(head);
    }
    Iterator end() const
    {
        return Iterator(nullptr);
    }
};
//конец первой ргр 
string int_to_char(long long int num_)
{
    string res;
    if (num_ < 0)
    {
        res += '-';
        num_ = -num_;
    }
    if (num_ == 0)
    {
        res += '0';
    }
    while (num_ != 0)
    {
        char digit = num_ % 10 + '0';
        res += digit;
        num_ /= 10;
    }
    int length = res.length();
    for (int i = 0; i < length / 2; i++)
        std::swap(res[i], res[length - 1 - i]);

    return res;
}

// Перечисление содержит константы для обозначения классов символьных лексем: БУКВА, ЦИФРА, ОПЕРАЦИЯ, ОТНОШ, ПРОБЕЛ, LF, ;, ОШИБКА, КОНЕЦ.
enum SymbolicTokenClass { Letter, Digit, Operation, Relation, Space, EndOfString, Dot_Comma, Error, EndOfFile };

// ЛЕКСЕМЫ
enum TokenClass { PUSH_, POP_, Operation_, Relation_, JMP_, JI_, READ_, WRITE_, END_, Comment_, Error_, EndOfFile_, NOTHING_, TRANSPOS_, BOOLMATRIX_, ITERATION_, INVERSION_ };

// Перечисление содержит константы для обозначения состояний автомата и одну дополнительную константу s_Stop.
enum States {
    s_A1, s_A2, s_B1, s_C1, s_D1, s_E1, s_E2, s_E3, s_F1, s_F2, s_F3, s_G1, s_H1, s_I1, s_I2, s_J1,
    s_Stop
};

// Лексеме будем присваивать значение и номер строки, используется в процедурах автомата
struct Token
{
    TokenClass token_class;
    long long int value;
    size_t line_num;
    Token(TokenClass token_ = NOTHING_, size_t num_ = 0, long long int value_ = 0)
    {
        token_class = token_;
        value = value_;
        line_num = num_;
    }
};

// Структура для представления символьной лексемы.
struct SymbolicToken
{
    SymbolicTokenClass token_class; // Класс символьной лексемы.
    int value;                      // Значение символьной лексемы.
};

struct Variable
{
    string name;
    int int_value;
    bool type_boolmatrix;
    BoolMatrix boolmatrix_val;
    Variable()
    {
        name = "";
        type_boolmatrix = false;
        int_value = 0;
        BoolMatrix temp;
        boolmatrix_val = temp;
    }
};


const int state_number = 16; // Число состояний (без s_Stop).
const int class_number = 9;  // Число символов входного алфавита.


class Parser
{
    bool error_flag;

    vector <Token> tokens;

    SymbolicToken s; // Символьная лексема.

    TokenClass c;    //Регистр класса. служит для хранения класса лексемы
    int number;      // Регистр числа. используется для вычисления констант
    int relation;    //Регистр отношения. хранит информацию о первом символе отношения
    string var_name; //Регистр переменной. накапливает имя переменной
    int detection;   //Регистр обнаружения. хранит номер позиции в таблице обнаружения для поиска ключевых слов.
    int value;       // Регистр значения. хранит значения лексем
    size_t line_num; // Номер строки.хранит номер текущей строки в программе.

    bool const_flag;

    map <string, Variable> name_table;
    typedef int (Parser::* function_pointer)();
    function_pointer table[state_number][class_number]; // Таблица указателей на процедуры автомата.

    /* Функция транслитерации. Метод сделан статическим, потому что он не зависит от объекта класса Parser.
    * ch - символ
    * Возвращаемое значение - результат транслитерации символа ch
    */
    static SymbolicToken transliterator(int ch)
    {
        SymbolicToken s;
        s.value = 0;
        if (isalpha(ch)) //Буква?
        {
            s.token_class = Letter;
            s.value = ch;
        }
        else if (isdigit(ch)) //Цифра?
        {
            s.token_class = Digit;
            s.value = ch - '0';
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '&' || ch == '|')
        {
            s.token_class = Operation;
            s.value = ch;
        }
        else if (ch == '=' || ch == '!' || ch == '<' || ch == '>')
        {
            s.token_class = Relation;
            s.value = ch;
        }
        else if (ch == ' ' || ch == '\t')
        {
            s.token_class = Space;
        }
        else if (ch == '\n')
        {
            s.token_class = EndOfString;
        }
        else if (ch == ';')
        {
            s.token_class = Dot_Comma;
        }
        else if (ch == EOF)
        {
            s.token_class = EndOfFile;
        }
        else
        {
            s.token_class = Error;
        }
        return s;
    }
    //Процедура обработки ошибки
    int ErrorProc()
    {
        error_flag = true;
        c = Error_;
        Token t(c, line_num);

        tokens.push_back(t);

        cout << "Error line " << line_num << endl;
        return s_J1;
    }

    //Процедуры автомата

    //Ожидается команда в начале строки. Это первая команда программы.
    int A1()
    {
        return s_A1;
    }
    // СОЗДАТЬ_ЛЕКСЕМУ
    //Увеличить НОМЕР_СТРОКИ на 1
    int A1a()
    {
        Token t(c, line_num);

        ++line_num;
        tokens.push_back(t);
        return s_A1;
    }
    //Увеличить НОМЕР_СТРОКИ на 1
    int A1b()
    {
        ++line_num;
        return s_A1;
    }
    //Ожидается очередная (не первая) команда в начале строки.
    int A2()
    {
        return s_A2;
    }
    //Увеличить НОМЕР_СТРОКИ на 1
    int A2a()
    {
        ++line_num;
        return s_A2;
    }
    //СОЗДАТЬ_ЛЕКСЕМУ
    //Увеличить НОМЕР_СТРОКИ на 1
    int A2b()
    {
        Token t(c, line_num);

        ++line_num;
        tokens.push_back(t);
        return s_A2;
    }
    //ДОБАВИТЬ_КОНСТАНТУ
    //СОЗДАТЬ ЛЕКСЕМУ
    //Увеличить НОМЕР_СТРОКИ на 1
    int A2c()
    {
        if (const_flag == false)
        {
            Token t(c, line_num, number);
            tokens.push_back(t);
            ++line_num;
            return s_A2;
        }

        var_name = int_to_char(number);
        Variable var;
        var.name = var_name;
        var.int_value = number;
        for (auto it : name_table)
        {
            if (get<0>(it) == var_name)
            {
                //приведение типа в long long int
                Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

                tokens.push_back(t);
                ++line_num;
                return s_A2;
            }
        }

        name_table.insert(pair<string, Variable>(var_name, var));
        Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

        tokens.push_back(t);
        ++line_num;

        return s_A2;
    }
    //ДОБАВИТЬ_ПЕРЕМЕННУЮ
    //СОЗДАТЬ_ЛЕКСЕМУ
    //Увеличить НОМЕР_СТРОКИ на 1
    int A2d()
    {
        if (var_name == "push" || var_name == "pop" || var_name == "read" || var_name == "write" ||
            var_name == "end" || var_name == "jmp" || var_name == "ji" || var_name == "transpos")
        {
            error_flag = true;

            c = Error_;
            Token t(c, line_num);

            tokens.push_back(t);

            cout << "Error line " << line_num << endl;

            ++line_num;
            return s_A2;
        }

        Variable var;
        var.name = var_name;
        for (auto it : name_table)
        {
            if (get<0>(it) == var_name)
            {
                Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

                tokens.push_back(t);
                ++line_num;
                return s_A2;
            }
        }

        name_table.insert(pair<string, Variable>(var_name, var));
        Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

        tokens.push_back(t);

        ++line_num;
        return s_A2;
    }
    //Если РЕГИСТР_ОТНОШЕНИЯ = "!", то перейти на процедуру, обрабатывающую ошибку.
    //СОЗДАТЬ_ЛЕКСЕМУ
    //Увеличить НОМЕР_СТРОКИ на 1
    int A2e()
    {
        if (relation == '!')
        {
            return ErrorProc();
        }

        Token t(c, line_num, relation);

        tokens.push_back(t);
        ++line_num;
        return s_A2;
    }
    //РЕГИСТР_КЛАССА = ОШИБКА
    //СОЗДАТЬ_ЛЕКСЕМУ
    //Увеличить НОМЕР_СТРОКИ на 1
    int A2f()
    {
        error_flag = true;

        c = Error_;
        Token t(c, line_num);

        tokens.push_back(t);

        cout << "Error line " << line_num << endl;

        ++line_num;
        return s_A2;
    }
    //Пользуясь значением входного символа как индексом, получить из начального вектора указатель и загрузить его в РЕГИСТР_ОБНАРУЖЕНИЯ.
    //Если РЕГИСТР_ОБНАРУЖЕНИЯ = 0, то перейти на процедуру обработки ошибки
    int B1a()
    {
        switch (s.value)
        {
        case 'e':
        {
            detection = 1;
            break;
        }
        case 'j':
        {
            detection = 3;
            break;
        }
        case 'p':
        {
            detection = 6;
            break;
        }
        case 'r':
        {
            detection = 11;
            break;
        }
        case 'w':
        {
            detection = 14;
            break;
        }
        case 't':
        {
            detection = 18;
            break;
        }
        case 'b':
        {
            detection = 25;
            break;
        }
        case 'i':
        {
            detection = 34;
            break;
        }
        default:
        {
            return ErrorProc();
        }
        }
        return s_B1;
    }
    //Увеличить РЕГИСТР_ОБНАРУЖЕНИЯ на 1.
    int B1b()
    {
        ++detection;
        return s_B1;
    }
    //Ожидается конец строки после правильной команды.
    int C1()
    {
        return s_C1;
    }
    //РЕГИСТР_КЛАССА = АРИФМЕТИЧЕСКАЯ_ОПЕРАЦИЯ
    //РЕГИСТР_ЗНАЧЕНИЯ = значение текущего входного символа
    //СОЗДАТЬ_ЛЕКСЕМУ
    int C1a()
    {
        c = Operation_;
        value = s.value;
        Token t(c, line_num, value);

        tokens.push_back(t);

        return s_C1;
    }
    //РЕГИСТР_КЛАССА = END
    //СОЗДАТЬ_ЛЕКСЕМУ
    int C1b()
    {
        c = END_;
        Token t(c, line_num);

        tokens.push_back(t);

        return s_C1;
    }
    //РЕГИСТР_КЛАССА = READ
    //СОЗДАТЬ_ЛЕКСЕМУ
    int C1c()
    {
        c = READ_;
        Token t(c, line_num);

        tokens.push_back(t);

        return s_C1;
    }
    //РЕГИСТР_КЛАССА = WRITE
    //СОЗДАТЬ_ЛЕКСЕМУ
    int C1d()
    {
        c = WRITE_;
        Token t(c, line_num);

        tokens.push_back(t);

        return s_C1;
    }
    //ДОБАВИТЬ_КОНСТАНТУ
    //СОЗДАТЬ_ЛЕКСЕМУ
    int C1e()
    {
        if (const_flag == false)
        {
            Token t(c, line_num, number);
            tokens.push_back(t);
            return s_C1;
        }

        var_name = int_to_char(number);
        Variable var;
        var.name = var_name;
        var.int_value = number;
        for (auto it : name_table)
        {
            if (get<0>(it) == var_name)
            {
                Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

                tokens.push_back(t);

                return s_C1;
            }
        }

        name_table.insert(pair<string, Variable>(var_name, var));
        Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

        tokens.push_back(t);

        return s_C1;
    }
    //ЛОБАВИТЬ_ПЕРЕМЕННУЮ
    //СОЗДАТЬ_лЕКСЕМУ
    int C1f()
    {
        if (var_name == "push" || var_name == "pop" || var_name == "read" || var_name == "write" ||
            var_name == "end" || var_name == "jmp" || var_name == "ji" || var_name == "transpos")
        {
            error_flag = true;

            c = Error_;
            Token t(c, line_num);

            tokens.push_back(t);

            cout << "Error line " << line_num << endl;

            return s_C1;
        }

        Variable var;
        var.name = var_name;
        for (auto it : name_table)
        {
            if (get<0>(it) == var_name)
            {
                Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

                tokens.push_back(t);

                return s_C1;
            }
        }

        name_table.insert(pair<string, Variable>(var_name, var));
        Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

        tokens.push_back(t);

        return s_C1;
    }
    //Если РЕГИСТР_ОТНОШЕНИЯ = "!", то перейти на процедуру, обрабатывающую ошибку.
    //СОЗДАТЬ_ЛЕКСЕМУ
    int C1g()
    {
        if (relation == '!')
        {
            return ErrorProc();
        }

        Token t(c, line_num, relation);

        tokens.push_back(t);
        return s_C1;
    }
    //Используя РЕГИСТР_ОТНОШЕНИЯ и значение текущего символа, вычислить значение отношения и поместить его в РЕГИСТР_ОТНОШЕНИЯ.
    //Если РЕГИСТР_ОТНОШНЕИЯ = 0, то перейти на процедуру, обрабатывающую ошибку.
    //СОЗДАТЬ_ЛЕКСЕМУ
    int C1h()
    {
        switch (relation)
        {
        case 1:
        {
            switch (s.value)
            {
            case ' ':
            {
                value = 1;
                break;
            }
            default:
            {
                value = 0;
                break;
            }
            }
            break;
        }
        case '!':
        {
            switch (s.value)
            {
            case '=':
            {
                value = 2;
                break;
            }
            default:
            {
                value = 0;
                break;
            }
            }
            break;
        }
        case 3:
        {
            switch (s.value)
            {
            case '=':
            {
                value = 5;
                break;
            }
            case ' ':
            {
                value = 3;
                break;
            }
            default:
            {
                value = 0;
                break;
            }
            }
            break;
        }
        case 4:
        {
            switch (s.value)
            {
            case '=':
            {
                value = 6;
                break;
            }
            case ' ':
            {
                value = 4;
                break;
            }
            default:
            {
                value = 0;
                break;
            }
            }
            break;
        }
        default:
        {
            value = 0;
            break;
        }
        }
        if (value == 0)
        {
            return ErrorProc();
        }
        Token t(c, line_num, value);

        tokens.push_back(t);

        return s_C1;
    }

    int C1j()
    {
        c = TRANSPOS_;
        Token t(c, line_num);

        tokens.push_back(t);

        return s_C1;
    }

    int C1K()
    {
        c = ITERATION_;
        Token t(c, line_num);
        tokens.push_back(t);
        return s_C1;
    }

    int C1L()
    {
        c = INVERSION_;
        Token t(c, line_num);
        tokens.push_back(t);
        return s_C1;
    }

    //РЕГИСТР_КЛАССА = ОТНОШЕНИЕ
    //РЕГИСТР_ОТНОШЕНИЯ = значение текущего входного символа
    int D1a()
    {
        c = Relation_;
        switch (s.value)
        {
        case '=':
        {
            relation = 1;
            break;
        }
        case '<':
        {
            relation = 3;
            break;
        }
        case '>':
        {
            relation = 4;
            break;
        }
        default:
        {
            relation = s.value;
            break;
        }
        }
        return s_D1;
    }
    //РЕГИСТР_КЛАССА = PUSH
    //ФЛАГ_КОНСТАНТЫ = 1
    int E1a()
    {
        c = PUSH_;
        const_flag = true;

        return s_E1;
    }
    //РЕГИСТР_КЛАССА = JI
    //ФЛАГ_КОНСТАНТЫ = 0
    int E2a()
    {
        c = JI_;
        const_flag = false;

        return s_E2;
    }
    //РЕГИСТР_КЛАССА = JMP
    //ФЛАГ_КОНСТАНТЫ = 0
    int E2b()
    {
        c = JMP_;
        const_flag = false;

        return s_E2;
    }

    int E3c()
    {
        c = BOOLMATRIX_;

        return s_E3;
    }

    //РЕГИСТР_КЛАССА = POP
    int E3a()
    {
        c = POP_;

        return s_E3;
    }
    //Ожидается первый символ переменной или константы (после push)
    int F1()
    {
        return s_F1;
    }

    //Ожидается начало константы (после jmp или ji)
    int F2()
    {
        return s_F2;
    }
    //Ожидается начало переменной (после pop)
    int F3()
    {
        return s_F3;
    }
    //РЕГИСТР_ЧИСЛА = значение текущего входного символа
    int G1a()
    {
        number = s.value;
        return s_G1;
    }
    //РЕГИСТР_ЧИСЛА = 10 * РЕГИСТР_ЧИСЛА + значение текущего входного символа.
    int G1b()
    {
        number = 10 * number + s.value;
        return s_G1;
    }
    //РЕГИСТР_ПЕРЕМЕННОЙ = значение текущего символа
    int H1a()
    {
        var_name = s.value;
        return s_H1;
    }
    //Добавить текущей входной символ в конец строки РЕГИСТР_ПЕРЕМЕННОЙ
    int H1b()
    {
        var_name.push_back(s.value);
        return s_H1;
    }
    //Считывается комментарий, перед который не было команд
    int I1()
    {
        return s_I1;
    }
    //РЕГИСТР_КЛАССА = КОММЕНТАРИЙ
    int I1a()
    {
        c = Comment_;
        return s_I1;
    }
    //Считывается комментарий, перед который были команды
    int I2()
    {
        return s_I2;
    }
    //РЕГИСТР_КЛАССА = КОММЕНТАРИЙ
    int I2a()
    {
        c = Comment_;
        return s_I2;
    }
    //ДОБАВИТЬ_КОНСТАНТУ
    //СОЗДАТЬ_ЛЕКСЕМУ
    //РЕГИСТР_КЛАССА = КОММЕНТАРИЙ
    int I2b()
    {
        if (const_flag == false)
        {
            Token t(c, line_num, number);
            tokens.push_back(t);
            c = Comment_;
            return s_I2;
        }

        var_name = int_to_char(number);
        Variable var;
        var.name = var_name;
        var.int_value = number;
        for (auto it : name_table)
        {
            if (get<0>(it) == var_name)
            {
                Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

                tokens.push_back(t);

                c = Comment_;
                return s_I2;
            }
        }

        name_table.insert(pair<string, Variable>(var_name, var));
        Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

        tokens.push_back(t);

        c = Comment_;
        return s_I2;
    }
    //ДОБАВИТЬ_ПЕРЕМЕННУЮ
    //СОЗДАТЬ_ЛЕКСЕМУ
    //РЕГИСТР_КЛАССА = КОММЕНТАРИЙ
    int I2c()
    {
        if (var_name == "push" || var_name == "pop" || var_name == "read" || var_name == "write" ||
            var_name == "end" || var_name == "jmp" || var_name == "ji" || var_name == "transpos")
        {
            error_flag = true;

            c = Error_;
            Token t(c, line_num);

            tokens.push_back(t);

            cout << "Error line " << line_num << endl;

            return s_I2;
        }

        Variable var;
        var.name = var_name;
        for (auto it : name_table)
        {
            if (get<0>(it) == var_name)
            {
                Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

                tokens.push_back(t);

                c = Comment_;
                return s_I2;
            }
        }

        name_table.insert(pair<string, Variable>(var_name, var));
        Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

        tokens.push_back(t);

        c = Comment_;
        return s_I2;
    }
    //Если РЕГИСТР_ОТНОШЕНИЯ = "!", то перейти на процеду, обрабатывающую ошибку.
    //СОЗДАТЬ_ЛЕКСЕМУ
    //РЕГИСТР_КЛАССА = КОММЕНТАРИЙ
    int I2d()
    {
        if (relation == '!')
        {
            return ErrorProc();
        }

        Token t(c, line_num, relation);

        tokens.push_back(t);
        c = Comment_;
        return s_I2;
    }
    //Счмтывает остаток строки после ошибки до перехода на новую строку.
    int J1()
    {
        return s_J1;
    }
    //Сравнить значение символа, на который указывает РЕГИСТР_ОБНАРУЖЕНИЯ, со значением текущего входного символа.
    //Если значения равны, то перейти к процедуре, на которую указывает РЕГИСТР_ОБНАРУЖЕНИЯ.
    //В противном случае поместить в РЕГИСТР_ОБНАРУЖЕНИЯ альтернативное значение из таблицы обнаружения
    //Если РЕГИСТ_ОБНАРУЖЕНИЯ = 0, то перейти на процедуру, обрабатывающую ошибку.
    //В противном случае перейти на M1.
    int M1()
    {
        switch (detection)
        {
        case 1:
        {
            if (s.value == 'n')
                return B1b();
            return ErrorProc();
        }
        case 2:
        {
            if (s.value == 'd')
                return C1b();
            return ErrorProc();
        }
        case 3:
        {
            if (s.value == 'i')
                return E2a();
            detection = 4;
            return M1();
        }
        case 4:
        {
            if (s.value == 'm')
                return B1b();
            return ErrorProc();
        }
        case 5:
        {
            if (s.value == 'p')
                return E2b();
            return ErrorProc();
        }
        case 6:
        {
            if (s.value == 'o')
                return B1b();
            detection = 8;
            return M1();
        }
        case 7:
        {
            if (s.value == 'p')
                return E3a();
            detection = 11;
            return M1();
        }
        case 8:
        {
            if (s.value == 'u')
                return B1b();
            return ErrorProc();
        }
        case 9:
        {
            if (s.value == 's')
                return B1b();
            return ErrorProc();
        }
        case 10:
        {
            if (s.value == 'h')
                return E1a();
            return ErrorProc();
        }
        case 11:
        {
            if (s.value == 'e')
                return B1b();
            return ErrorProc();
        }
        case 12:
        {
            if (s.value == 'a')
                return B1b();
            return ErrorProc();
        }
        case 13:
        {
            if (s.value == 'd')
                return C1c();
            return ErrorProc();
        }
        case 14:
        {
            if (s.value == 'r')
                return B1b();
            return ErrorProc();
        }
        case 15:
        {
            if (s.value == 'i')
                return B1b();
            return ErrorProc();
        }
        case 16:
        {
            if (s.value == 't')
                return B1b();
            return ErrorProc();
        }
        case 17:
        {
            if (s.value == 'e')
                return C1d();
            return ErrorProc();
        }
        case 18:
        {
            if (s.value == 'r')
                return B1b();
            return ErrorProc();
        }
        case 19:
        {
            if (s.value == 'a')
                return B1b();
            return ErrorProc();
        }
        case 20:
        {
            if (s.value == 'n')
                return B1b();
            return ErrorProc();
        }
        case 21:
        {
            if (s.value == 's')
                return B1b();
            return ErrorProc();
        }
        case 22:
        {
            if (s.value == 'p')
                return B1b();
            return ErrorProc();
        }
        case 23:
        {
            if (s.value == 'o')
                return B1b();
            return ErrorProc();
        }
        case 24:
        {
            if (s.value == 's')
                return C1j();
            return ErrorProc();

        }
        case 25:
        {
            if (s.value == 'o')
                return B1b();
            return ErrorProc();
        }
        case 26:
        {
            if (s.value == 'o')
                return B1b();
            return ErrorProc();
        }
        case 27:
        {
            if (s.value == 'l')
                return B1b();
            return ErrorProc();
        }
        case 28:
        {
            if (s.value == 'm')
                return B1b();
            return ErrorProc();
        }
        case 29:
        {
            if (s.value == 'a')
                return B1b();
            return ErrorProc();
        }
        case 30:
        {
            if (s.value == 't')
                return B1b();
            return ErrorProc();
        }
        case 31:
        {
            if (s.value == 'r')
                return B1b();
            return ErrorProc();
        }
        case 32:
        {
            if (s.value == 'i')
                return B1b();
            return ErrorProc();
        }
        case 33:
        {
            if (s.value == 'x')
                return E3c();
            return ErrorProc();
        }
        case 34:
        {
            if (s.value == 't')
                return B1b();
            detection = 42;
            return M1();
        }
        case 35:
        {
            if (s.value == 'e')
                return B1b();
            return ErrorProc();
        }
        case 36:
        {
            if (s.value == 'r')
                return B1b();
            return ErrorProc();
        }
        case 37:
        {
            if (s.value == 'a')
                return B1b();
            return ErrorProc();
        }
        case 38:
        {
            if (s.value == 't')
                return B1b();
            return ErrorProc();
        }
        case 39:
        {
            if (s.value == 'i')
                return B1b();
            return ErrorProc();
        }
        case 40:
        {
            if (s.value == 'o')
                return B1b();
            return ErrorProc();
        }
        case 41:
        {
            if (s.value == 'n')
                return C1K();   // новое сост.
            return ErrorProc();
        }
        case 42:
        {
            if (s.value == 'n')
                return B1b();
            return ErrorProc();
        }
        case 43:
        {
            if (s.value == 'v')
                return B1b();
            return ErrorProc();
        }
        case 44:
        {
            if (s.value == 'e')
                return B1b();
            return ErrorProc();
        }
        case 45:
        {
            if (s.value == 'r')
                return B1b();
            return ErrorProc();
        }
        case 46:
        {
            if (s.value == 's')
                return B1b();
            return ErrorProc();
        }
        case 47:
        {
            if (s.value == 'i')
                return B1b();
            return ErrorProc();
        }
        case 48:
        {
            if (s.value == 'o')
                return B1b();
            return ErrorProc();
        }
        case 49:
        {
            if (s.value == 'n')
                return C1L();
            return ErrorProc();
        }
        default:
        {
            return ErrorProc();
        }
        }
    }
    //РЕГИСТР_КЛАССА = КОНЦЕВОЙ_МАРКЕР
    //СОЗДАТЬ_ЛЕКСЕМУ
    //Стоп
    int Exit1()
    {
        c = EndOfFile_;
        Token t(c, line_num);

        tokens.push_back(t);

        return s_Stop;
    }
    //Если РЕГИСТР_ОТНОШЕНИЯ = "!", то РЕГИСТР_КЛАССА = ОШИБКА
    //СОЗДАТЬ_ЛЕКСЕМУ
    //РЕГИСТР_КЛАССА = КОНЦЕВОЙ_МАРКЕР
    //СОЗДАТЬ_ЛЕКСЕМУ
    //Стоп
    int Exit2()
    {
        if (relation == '!')
        {
            error_flag = true;

            cout << "Error line " << line_num << endl;
            c = Error_;
            Token t(c, line_num);

            tokens.push_back(t);

            c = EndOfFile_;
            Token t1(c, line_num);

            tokens.push_back(t1);

            return s_Stop;
        }
        Token t(c, line_num, relation);
        tokens.push_back(t);

        c = EndOfFile_;
        Token t1(c, line_num);

        tokens.push_back(t1);

        return s_Stop;
    }
    //ДОБАВИТЬ_КОНСТАНТУ
    //СОЗДАТЬ_ЛЕКСЕМУ
    //РЕГИСТР_КЛАССА = КОНЦЕВОЙ_МАРКЕР
    //Стоп
    int Exit3()
    {
        if (const_flag == false)
        {
            Token t(c, line_num, number);
            tokens.push_back(t);

            c = EndOfFile_;
            Token t1(c, line_num);

            tokens.push_back(t1);
            return s_Stop;
        }

        var_name = int_to_char(number);
        Variable var;
        var.name = var_name;
        var.int_value = number;
        for (auto it : name_table)
        {
            if (get<0>(it) == var_name)
            {
                Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

                tokens.push_back(t);

                c = EndOfFile_;
                Token t1(c, line_num);

                tokens.push_back(t1);
                return s_Stop;
            }
        }

        name_table.insert(pair<string, Variable>(var_name, var));
        Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

        tokens.push_back(t);

        c = EndOfFile_;
        Token t1(c, line_num);

        tokens.push_back(t1);

        return s_Stop;
    }
    //ДОБАВИТЬ_ПЕРЕМЕННУЮ
    //СОЗДАТЬ_ЛЕКСЕМУ
    //РЕГИСТР_КЛАССА = КОНЦЕВОЙ_МАРКЕР
    //СОЗДАТЬ_ЛЕКСЕМУ
    //Стоп
    int Exit4()
    {
        if (var_name == "push" || var_name == "pop" || var_name == "read" || var_name == "write" ||
            var_name == "end" || var_name == "jmp" || var_name == "ji" || var_name == "transpos")
        {
            error_flag = true;

            c = Error_;
            Token t(c, line_num);

            tokens.push_back(t);

            cout << "Error line " << line_num << endl;

            return s_Stop;
        }

        Variable var;
        var.name = var_name;
        for (auto it : name_table)
        {
            if (get<0>(it) == var_name)
            {
                Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

                tokens.push_back(t);

                c = EndOfFile_;
                Token t1(c, line_num);
                tokens.push_back(t1);
                return s_Stop;
            }
        }

        name_table.insert(pair<string, Variable>(var_name, var));
        Token t(c, line_num, reinterpret_cast<long long int>(&name_table[var.name]));

        tokens.push_back(t);

        c = EndOfFile_;
        Token t1(c, line_num);
        tokens.push_back(t1);
        return s_Stop;
    }

    int Exit5()
    {
        Token t(c, line_num);
        tokens.push_back(t);

        c = EndOfFile_;
        Token t1(c, line_num);
        tokens.push_back(t1);
        return s_Stop;
    }
public:

    // Конструктор инициализирует таблицу указателей на процедуры автомата.
    Parser()
    {
        error_flag = false;
        c = EndOfFile_;
        const_flag = false;
        detection = 0;
        line_num = 1;
        number = 0;
        relation = 0;
        value = 0;
        s.token_class = EndOfFile;
        s.value = 0;

        // Сначала во все ячейки заносится указатель на процедуру обработки ошибок.
        for (int i = 0; i < state_number; i++)
        {
            for (int j = 0; j < class_number; j++)
            {
                table[i][j] = &Parser::ErrorProc;
            }
        }
        // Далее заполняются ячейки для остальных процедур.
        table[s_A1][Letter] = &Parser::B1a; table[s_A1][Operation] = &Parser::C1a; table[s_A1][Relation] = &Parser::D1a; table[s_A1][Space] = &Parser::A1; table[s_A1][EndOfString] = &Parser::A1b; table[s_A1][Dot_Comma] = &Parser::I1a;
        table[s_A2][Letter] = &Parser::B1a; table[s_A2][Operation] = &Parser::C1a; table[s_A2][Relation] = &Parser::D1a; table[s_A2][Space] = &Parser::A2; table[s_A2][EndOfString] = &Parser::A2a; table[s_A2][Dot_Comma] = &Parser::I2a; table[s_A2][EndOfFile] = &Parser::Exit1;
        table[s_B1][Letter] = &Parser::M1; table[s_B1][EndOfString] = &Parser::A2f;
        table[s_C1][Space] = &Parser::C1; table[s_C1][EndOfString] = &Parser::A2a; table[s_C1][Dot_Comma] = &Parser::I2a; table[s_C1][EndOfFile] = &Parser::Exit1;
        table[s_D1][Relation] = &Parser::C1h; table[s_D1][Space] = &Parser::C1g; table[s_D1][EndOfString] = &Parser::A2e; table[s_D1][Dot_Comma] = &Parser::I2d; table[s_D1][EndOfFile] = &Parser::Exit2;
        table[s_E1][Space] = &Parser::F1; table[s_E1][EndOfString] = &Parser::A2f;
        table[s_E2][Space] = &Parser::F2; table[s_E2][EndOfString] = &Parser::A2f;
        table[s_E3][Space] = &Parser::F3; table[s_E3][EndOfString] = &Parser::A2f;
        table[s_F1][Letter] = &Parser::H1a; table[s_F1][Digit] = &Parser::G1a; table[s_F1][Space] = &Parser::F1; table[s_F1][EndOfString] = &Parser::A2f;
        table[s_F2][Digit] = &Parser::G1a; table[s_F2][Space] = &Parser::F2; table[s_F2][EndOfString] = &Parser::A2f;
        table[s_F3][Letter] = &Parser::H1a; table[s_F3][Space] = &Parser::F3; table[s_F3][EndOfString] = &Parser::A2f;
        table[s_G1][Digit] = &Parser::G1b; table[s_G1][Space] = &Parser::C1e; table[s_G1][EndOfString] = &Parser::A2c; table[s_G1][Dot_Comma] = &Parser::I2b; table[s_G1][EndOfFile] = &Parser::Exit3;
        table[s_H1][Letter] = &Parser::H1b; table[s_H1][Digit] = &Parser::H1b; table[s_H1][Space] = &Parser::C1f; table[s_H1][EndOfString] = &Parser::A2d; table[s_H1][Dot_Comma] = &Parser::I2c; table[s_H1][EndOfFile] = &Parser::Exit4;
        table[s_I1][Letter] = &Parser::I1; table[s_I1][Digit] = &Parser::I1; table[s_I1][Operation] = &Parser::I1; table[s_I1][Relation] = &Parser::I1; table[s_I1][Space] = &Parser::I1; table[s_I1][EndOfString] = &Parser::A1a; table[s_I1][Dot_Comma] = &Parser::I1; table[s_I1][Error] = &Parser::I1;
        table[s_I2][Letter] = &Parser::I2; table[s_I2][Digit] = &Parser::I2; table[s_I2][Operation] = &Parser::I2; table[s_I2][Relation] = &Parser::I2; table[s_I2][Space] = &Parser::I2; table[s_I2][EndOfString] = &Parser::A2b; table[s_I2][Dot_Comma] = &Parser::I2; table[s_I2][Error] = &Parser::I2; table[s_I2][EndOfFile] = &Parser::Exit5;
        table[s_J1][Letter] = &Parser::J1; table[s_J1][Digit] = &Parser::J1; table[s_J1][Operation] = &Parser::J1; table[s_J1][Relation] = &Parser::J1; table[s_J1][Space] = &Parser::J1; table[s_J1][EndOfString] = &Parser::A2a; table[s_J1][Dot_Comma] = &Parser::J1; table[s_J1][Error] = &Parser::J1; table[s_J1][EndOfFile] = &Parser::Exit1;
    }

    // Основная функция анализа. Функция запускает конечный автомат на тексте из файла filename.
    void parse(const char* filename)
    {
        ifstream in(filename);
        if (!in)
        {
            cout << "File not opened " << filename << endl;
            return;
        }

        int ch; // Входной символ.
        int state = s_A1;  // Текущее состояние автомата.

        while (state != s_Stop)
        {
            // s_Stop - техническое состояние. Когда автомат попадает в s_Stop, цикл останавливается.
            ch = in.get();
            s = transliterator(ch);
            state = (this->*table[state][s.token_class])();
        }

        in.close();
    }

    void Print_Tokens()
    {
        for (Token t : tokens)
        {
            cout << t.line_num << ": ";
            switch (t.token_class)
            {
            case PUSH_:
            {
                cout << "PUSH";
                break;
            }
            case POP_:
            {
                cout << "POP";
                break;
            }
            case Operation_:
            {
                cout << "Operation";
                break;
            }
            case Relation_:
            {
                cout << "Relation";
                break;
            }
            case JMP_:
            {
                cout << "JMP";
                break;
            }
            case JI_:
            {
                cout << "JI";
                break;
            }
            case READ_:
            {
                cout << "READ";
                break;
            }
            case WRITE_:
            {
                cout << "WRITE";
                break;
            }
            case END_:
            {
                cout << "END";
                break;
            }
            case Comment_:
            {
                cout << "Comment";
                break;
            }
            case Error_:
            {
                cout << "Error";
                break;
            }
            case EndOfFile_:
            {
                cout << "EOF";
                break;
            }
            case TRANSPOS_:
            {
                cout << "TRANSPOS";
                break;
            }
            case BOOLMATRIX_:
            {
                cout << "boolmatrix";
                break;
            }
            case ITERATION_:
            {
                cout << "iteration";
                break;
            }
            case INVERSION_:
            {
                cout << "inversion";
                break;
            }
            default:
            {
                cout << t.token_class;
                break;
            }
            }
            cout << " = " << t.value << endl;
        }
        cout << endl << "Variables:" << endl;

        for (auto it : name_table)
            if (isalpha(get<0>(it)[0]))
                cout << get<0>(it) << "; ";

        cout << endl << "Constants:" << endl;

        for (auto it : name_table)
            if (isdigit(get<0>(it)[0]))
                cout << get<0>(it) << "; ";

        cout << endl;
    }

    void Interpretator()
    {
        if (error_flag == true)
            return;

        Token t;
        stack <Variable> stack_;
        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            t = (*it);
            switch (t.token_class)
            {
            case PUSH_:
            {
                stack_.push(*(Variable*)t.value);
                break;
            }
            case POP_:
            {
                (*(Variable*)t.value) = stack_.top();
                stack_.pop();
                break;
            }
            case Operation_:
            {
                Variable second_arg = stack_.top();
                stack_.pop();
                Variable first_arg = stack_.top();
                stack_.pop();
                if ((first_arg.type_boolmatrix == 0) && (second_arg.type_boolmatrix == 0))
                {
                    switch (t.value)
                    {
                    case'+':
                    {
                        int r = first_arg.int_value + second_arg.int_value;
                        string name = int_to_char(r);
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case'-':
                    {
                        int r = first_arg.int_value - second_arg.int_value;
                        string name = int_to_char(r);
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case'*':
                    {
                        int r = first_arg.int_value * second_arg.int_value;
                        string name = int_to_char(r);
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case'/':
                    {
                        int r = first_arg.int_value / second_arg.int_value;
                        string name = int_to_char(r);
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case '%':
                    {
                        int r = first_arg.int_value % second_arg.int_value;
                        string name = int_to_char(r);
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    default:
                        return;
                    }
                }

                if ((first_arg.type_boolmatrix == 1) && (second_arg.type_boolmatrix == 1))
                {
                    switch (t.value)
                    {
                    case'*':
                    {
                        BoolMatrix r = first_arg.boolmatrix_val * second_arg.boolmatrix_val;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.type_boolmatrix = true;
                        res.boolmatrix_val = r;
                        stack_.push(res);
                        break;
                    }
                    case'&':
                    {
                        BoolMatrix r = first_arg.boolmatrix_val & second_arg.boolmatrix_val;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.type_boolmatrix = true;
                        res.boolmatrix_val = r;
                        stack_.push(res);
                        break;
                    }
                    case'|':
                    {
                        BoolMatrix r = first_arg.boolmatrix_val | second_arg.boolmatrix_val;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.type_boolmatrix = true;
                        res.boolmatrix_val = r;
                        stack_.push(res);
                        break;
                    }
                    default:
                        return;
                    }
                }
                break;
            }
            case Relation_:
            {
                Variable second_arg = stack_.top();
                stack_.pop();
                Variable first_arg = stack_.top();
                stack_.pop();

                if ((first_arg.type_boolmatrix == 0) && (second_arg.type_boolmatrix == 0))
                {
                    switch (t.value)
                    {
                    case 1:
                    {
                        bool r = first_arg.int_value == second_arg.int_value;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case 2:
                    {
                        bool r = first_arg.int_value != second_arg.int_value;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case 3:
                    {
                        bool r = first_arg.int_value < second_arg.int_value;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case 4:
                    {
                        bool r = first_arg.int_value > second_arg.int_value;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case 5:
                    {
                        bool r = first_arg.int_value <= second_arg.int_value;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case 6:
                    {
                        bool r = first_arg.int_value >= second_arg.int_value;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    default:
                        return;
                    }
                }

                if ((first_arg.type_boolmatrix == 1) && (second_arg.type_boolmatrix == 1))
                {
                    switch (t.value)
                    {
                    case 1:
                    {
                        bool r = first_arg.boolmatrix_val == second_arg.boolmatrix_val;
                        string name = "";
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    case 2:
                    {
                        bool r = first_arg.boolmatrix_val != second_arg.boolmatrix_val;
                        string name = int_to_char(r);
                        Variable res;
                        res.name = name;
                        res.int_value = r;
                        stack_.push(res);
                        break;
                    }
                    default:
                        return;
                    }
                }
                break;
            }
            case JMP_:
            {
                if (t.value > line_num)
                    return;

                for (it = tokens.begin(); (*it).line_num != t.value;)
                    ++it;
                --it;
                break;
            }
            case JI_:
            {

                if (stack_.top().int_value == 0)
                {
                    stack_.pop();
                    break;
                }

                if (t.value > line_num)
                    return;

                for (it = tokens.begin(); (*it).line_num != t.value;)
                    ++it;
                --it;
                break;
            }
            case READ_:
            {
                int value;
                cin >> value;
                cout << endl;
                if (value < 0)
                    return;
                string name = int_to_char(value);
                Variable res;
                res.name = name;
                res.int_value = value;
                stack_.push(res);
                break;
            }
            case WRITE_:
            {
                Variable res = stack_.top();
                stack_.pop();
                if (res.type_boolmatrix == 0)
                    cout << res.int_value << endl;
                if (res.type_boolmatrix == 1)
                    cout << res.boolmatrix_val << endl;

                break;
            }
            case END_:
                return;
            case Comment_:
                break;
            case Error_:
                return;
            case EndOfFile_:
                return;
            case BOOLMATRIX_:
            {
                BoolMatrix matrix;
                cin >> matrix;

                (*(Variable*)t.value).name = "";
                (*(Variable*)t.value).type_boolmatrix = true;
                (*(Variable*)t.value).boolmatrix_val = matrix;
                stack_.push((*(Variable*)t.value));
                break;
            }
            case TRANSPOS_:
            {
                if (stack_.top().type_boolmatrix == 0)
                    return;
                Variable res;
                BoolMatrix a;

                res.name = "";
                res.boolmatrix_val = stack_.top().boolmatrix_val;
                stack_.pop();
                res.boolmatrix_val = res.boolmatrix_val.transpose();

                res.type_boolmatrix = true;
                stack_.push(res);
                break;
            }
            case ITERATION_:
            {
                if (stack_.top().type_boolmatrix == 0)
                    return;
                Variable res;
                BoolMatrix a;

                res.name = "";
                res.boolmatrix_val = stack_.top().boolmatrix_val;
                stack_.pop();
                res.boolmatrix_val = res.boolmatrix_val.iteration();

                res.type_boolmatrix = true;
                stack_.push(res);
                break;
            }
            case INVERSION_:
            {
                if (stack_.top().type_boolmatrix == 0)
                    return;
                Variable res;
                BoolMatrix a;

                res.name = "";
                res.boolmatrix_val = stack_.top().boolmatrix_val;
                stack_.pop();
                res.boolmatrix_val = res.boolmatrix_val.inversion();

                res.type_boolmatrix = true;
                stack_.push(res);
                break;
            }
            default:
                return;
            }
        }
    }
};

int main()
{
    Parser p;
    p.parse("input8.txt");
    p.Print_Tokens();
    p.Interpretator();



    return 0;
}
