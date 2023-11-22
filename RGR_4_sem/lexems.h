#ifndef _LEXEMS_H__
#define _LEXEMS_H__

#include <iostream>
enum class LexemType
{
    Identifier, Semicolon, Const,
    Let, To, While, Do, For, Type, ConstRgr1TypeBegin, ConstRgr1TypeEnd,
    LeftBracket, RightBracket, LeftSquareBracket, RightSquareBracket, If,
    Else, Read, Write, Jump, When, End, Abort, Case, EndIn, Comment, Comma, Func,
    Operator, Relation, Otherwise, Label, Then, DoubleDot, Assignment
};
class Lexem
{
public:
    Lexem(LexemType type_, const std::string& name_)
        : type(type_), name(name_) {}

    LexemType GetLexemType() const { return type; }
    std::string GetName() const { return name; }
    static std::string LexemTypeToString(LexemType type)
    {
        switch (type)
        {
        case LexemType::Identifier: return "Идентификатор";
        case LexemType::Semicolon: return "Пустой оператор";
        case LexemType::Const: return "Константа";
        case LexemType::While: case LexemType::Do:
        case LexemType::For: case LexemType::Else:
        case LexemType::Then: case LexemType::To:
        case LexemType::End: case LexemType::If:
        case LexemType::Let: case LexemType::Jump:
            return "Ключевое слово";
        case LexemType::Operator: case LexemType::Read:
        case LexemType::Write: case LexemType::Comma: case LexemType::Assignment:
            return "Оператор";
        case LexemType::Relation: return "Отношение";
        case LexemType::Label: return "Метка";
        case LexemType::LeftBracket: case LexemType::RightBracket:
        case LexemType::LeftSquareBracket: case LexemType::RightSquareBracket:
            return "Скобка";
        case LexemType::Abort: return "Исключение";
        case LexemType::Func: return "Функция для типа";
        case LexemType::ConstRgr1TypeBegin: return "Константа типа начало";
        case LexemType::ConstRgr1TypeEnd: return "Константа типа конец";
        case LexemType::Type: return "Тип данных";
        case LexemType::Comment: return "Комментарий";
        default:
            return "Без названия";
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const Lexem& lexem)
    {
        std::cout << "Лексема: " << LexemTypeToString(lexem.type)
            << "; Значение: " << lexem.name;
        return os;
    }
public:
    LexemType type;
    std::string name;
};
#endif