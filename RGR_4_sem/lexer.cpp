#include <cctype>
#include <algorithm>
#include <iostream>
#include "lexer.h"
using std::cout;
using std::endl;
bool Lexer::isSpace(char ch)
{
    return ch == ' ';
}
bool Lexer::isNewLine(char ch)
{
    return ch == '\n';
}
bool Lexer::isRelationSymbol(char ch)
{
    return ch == '=' || ch == '<' || ch == '>';
}
bool Lexer::isOperator(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/'
        || ch == '%';
}
bool Lexer::isEndOfFile(char ch)
{
    return ch == '\0';
}
int Lexer::transliterator(char ch)
{
    if (isalpha(ch)) return Letter;
    if (isdigit(ch)) return Digit;
    if (isSpace(ch)) return Space;
    if (isNewLine(ch)) return NewLine;
    if (isOperator(ch)) return Operator;
    if (ch == ';') return Semicolon;
    if (isRelationSymbol(ch)) return Relation;
    if (isEndOfFile(ch)) return EndOfFile;
    if (ch == '[') return LeftSquareBracket;
    if (ch == ']') return RightSquareBracket;
    if (ch == '(') return LeftBracket;
    if (ch == ')') return RightBracket;
    if (ch == ',') return Comma;
    if (ch == ':') return DoubleDot;
    return OtherSymbol;
}
std::vector<Lexem> Lexer::Start()
{
    const char* text = programm.c_str();
    for (; currentPos < programm.length() + 1; ++currentPos)
    {
        currentSymbol = text[currentPos];
        (this->*states[state][transliterator(currentSymbol)])();
    }
    return lexems;
}
void Lexer::removeLexemsConstType()
{
    while (lexems.back().GetLexemType() != LexemType::ConstRgr1TypeBegin)
        lexems.pop_back();
    lexems.pop_back();
}
void Lexer::createOperator()
{
    if (register_operator == '-' && programm.c_str()[currentPos + 1] == '>')
    {
        lexems.emplace_back(LexemType::Write, "write");
        ++currentPos;
    }
    else
    {
        lexems.emplace_back(LexemType::Operator, std::string(1, register_operator));
    }
}
void Lexer::createSemicolon()
{
    lexems.emplace_back(LexemType::Semicolon, std::string(1, ';'));
}
void Lexer::createLeftSquareBracket()
{
    lexems.emplace_back(LexemType::LeftSquareBracket, std::string(1, '['));
}
void Lexer::createRightSquareBracket()
{
    lexems.emplace_back(LexemType::RightSquareBracket, std::string(1, ']'));
}
void Lexer::createRelation()
{
    lexems.emplace_back(LexemType::Relation, register_relation);
}
void Lexer::createOperatorArrowIn()
{
    lexems.emplace_back(LexemType::Read, "<-");
}
void Lexer::createOperatorArrowOut()
{
    lexems.emplace_back(LexemType::Write, "->");
}
void Lexer::createConst()
{
    lexems.emplace_back(LexemType::Const, register_name);
}
void Lexer::createLeftBracket()
{
    lexems.emplace_back(LexemType::LeftBracket, std::string(1, '('));
}
void Lexer::createRightBracket()
{
    lexems.emplace_back(LexemType::RightBracket, std::string(1, ')'));
}
void Lexer::createComma()
{
    lexems.emplace_back(LexemType::Comma, std::string(1, ','));
}
void Lexer::createAssignment()
{
    lexems.emplace_back(LexemType::Assignment, std::string(1, '='));
}
void Lexer::createLabel()
{
    lexems.emplace_back(LexemType::Label, register_name);
}
void Lexer::createDoubleDot()
{
    lexems.emplace_back(LexemType::DoubleDot, std::string(1, ':'));
}
void Lexer::createConstType(LexemType type)
{
    lexems.emplace_back(type, "");
}
void Lexer::createComment()
{
    lexems.emplace_back(LexemType::Comment, "comment");
}
Lexer::Lexer(const std::string& programm_) : programm(programm_)
{
    state = s_q0;
    for (int i = 0; i < StatesCount; ++i)
    {
        for (int j = 0; j < TokensCount; ++j)
        {
            states[i][j] = &Lexer::q0;
        }
    }
    initErrorStates();
    initStates();
}
void Lexer::initErrorStates()
{
    for (int i = 0; i < TokensCount; ++i)
    {
        states[s_Err][i] = &Lexer::skipError;
    }
    states[s_Err][NewLine] = &Lexer::newLine;
    states[s_Err][Space] = &Lexer::q0;
    states[s_Err][EndOfFile] = &Lexer::q0;
}
void Lexer::initLabelState()
{
    for (int i = 0; i < TokensCount; ++i)
    {
        states[s_Label][i] = &Lexer::errorLabelName;
    }
    states[s_Label][Letter] = &Lexer::label;
    states[s_Label][Digit] = &Lexer::label;
    states[s_Label][Space] = &Lexer::labelCreate;
    states[s_Label][NewLine] = &Lexer::labelCreateNewLine;
    states[s_Label][EndOfFile] = &Lexer::labelCreate;
}
void Lexer::initD1State()
{
    for (int i = 0; i < TokensCount; ++i)
    {
        states[s_D1][i] = &Lexer::constCreateAndBack;
    }
    states[s_D1][Digit] = &Lexer::D1;
    states[s_D1][Letter] = &Lexer::errorConst;
    states[s_D1][Space] = &Lexer::constCreate;
    states[s_D1][EndOfFile] = &Lexer::constCreate;
}
void Lexer::initq0State()
{
    for (int j = 0; j < TokensCount; ++j)
    {
        states[s_q0][j] = &Lexer::errorUnexpectedSymbol;
    }
    states[s_q0][Letter] = &Lexer::L1;
    states[s_q0][Digit] = &Lexer::D1;
    states[s_q0][Operator] = &Lexer::Op1;
    states[s_q0][Relation] = &Lexer::Rel1;
    states[s_q0][LeftBracket] = &Lexer::leftBracket;
    states[s_q0][RightBracket] = &Lexer::rightBracket;
    states[s_q0][LeftSquareBracket] = &Lexer::leftSquareBracket;
    states[s_q0][RightSquareBracket] = &Lexer::rightSquareBracket;
    states[s_q0][NewLine] = &Lexer::newLine;
    states[s_q0][Semicolon] = &Lexer::semicolon;
    states[s_q0][Comma] = &Lexer::comma;
    states[s_q0][DoubleDot] = &Lexer::doubleDot;
    states[s_q0][EndOfFile] = &Lexer::q0;
    states[s_q0][Space] = &Lexer::q0;
}
void Lexer::initConstTypeState()
{
    for (int i = 0; i < TokensCount; ++i)
    {
        states[s_ConstType][i] = &Lexer::errorConstType;
    }
    states[s_ConstType][Operator] = &Lexer::constTypeCheckIfEndAndCreateConstIfNot;
    states[s_ConstType][NewLine] = &Lexer::constTypeErrorNewLine;
    states[s_ConstType][Digit] = &Lexer::constTypeValue;
}
void Lexer::initL1State()
{
    for (int i = 0; i < TokensCount; ++i)
    {
        states[s_L1][i] = &Lexer::LCreateAndBack;
    }
    states[s_L1][Letter] = &Lexer::L1;
    states[s_L1][Digit] = &Lexer::L1;
    states[s_L1][Space] = &Lexer::LCreate;
    states[s_L1][EndOfFile] = &Lexer::LCreate;
    states[s_L1][OtherSymbol] = &Lexer::L1;
}
void Lexer::initRel1State()
{
    for (int j = 0; j < TokensCount; ++j)
    {
        states[s_Rel1][j] = &Lexer::RelCreateAndBack;
    }
    states[s_Rel1][Relation] = &Lexer::RelCreateDouble;
    states[s_Rel1][Operator] = &Lexer::Rel1OperatorArrowIn;
}
void Lexer::initRel2State()
{
    for (int j = 0; j < TokensCount; ++j)
    {
        states[s_Rel2][j] = &Lexer::errorRelation;
    }
    states[s_Rel2][Relation] = &Lexer::CommentCheck;
}
void Lexer::initCommentState()
{
    for (int j = 0; j < TokensCount; ++j)
    {
        states[s_Comment][j] = &Lexer::comment;
    }
    states[s_Comment][Relation] = &Lexer::CommentEnd1;
}
void Lexer::initCommentEnd()
{
    for (int j = 0; j < TokensCount; ++j)
    {
        states[s_CommentEnd][j] = &Lexer::comment;
    }
    states[s_CommentEnd][Relation] = &Lexer::CommentEnd2;
}
void Lexer::initCommentEnd2()
{
    for (int j = 0; j < TokensCount; ++j)
    {
        states[s_CommentEnd2][j] = &Lexer::comment;
    }
    states[s_CommentEnd2][Relation] = &Lexer::CommentEnd3;
}
void Lexer::initOp1State()
{
    for (int j = 0; j < TokensCount; ++j)
    {
        states[s_Op1][j] = &Lexer::OpCreateAndBack;
    }
    states[s_Op1][Operator] = &Lexer::OpMaybeCommentOrError;
    states[s_Op1][Relation] = &Lexer::OpifArrowOrBack;
}
void Lexer::initStates()
{
    initq0State();
    initD1State();
    initL1State();
    initRel1State();
    initRel2State();
    initCommentState();
    initCommentEnd();
    initCommentEnd2();
    initOp1State();
    initLabelState();
    initConstTypeState();
}
void Lexer::errorLabelEnd()
{
    state = s_Err;
    cout << "Ошибка на строке " << currentLine <<
        ": Метка не закрывается '<<'" << endl;
    register_name.clear();
}
void Lexer::errorUnexpectedSymbol()
{
    state = s_Err;
    cout << "Ошибка на строке " << currentLine <<
        ": Неожиданный символ '" << currentSymbol << "'" << endl;
}
void Lexer::errorLabelName()
{
    state = s_Err;
    cout << "Ошибка на строке " << currentLine <<
        ": имя метки не может содержать символ '" <<
        currentSymbol << "'" << endl;
}
void Lexer::errorConst()
{
    state = s_Err;
    cout << "Ошибка на строке " << currentLine <<
        ": идентификатор не может начинаться с числа" << endl;
    register_name.clear();
}
void Lexer::errorRelation()
{
    state = s_q0;
    cout << "Ошибка на строке " << currentLine <<
        ": Недопустимое сочетание символов отношения '" << register_relation
        << "'" << endl;
    register_relation.clear();
}
void Lexer::errorOperator()
{
    state = s_q0;
    cout << "Ошибка на строке " << currentLine <<
        ": Нет оператора '" << register_operator << currentSymbol
        << "'" << endl;
}
void Lexer::errorConstType()
{
    state = s_Err;
    cout << "Ошибка на строке " << currentLine <<
        ": Недопустимый символ '" << currentSymbol << "'" << endl;
    removeLexemsConstType();
    register_name.clear();
}
void Lexer::errroConstTypeContinue()
{
    state = s_Err;
    cout << "Ошибка на строке " << currentLine <<
        ": Недопустимый символ для значений константы типа" <<
        register_name.back() << endl;
    removeLexemsConstType();
    register_name.clear();
}
void Lexer::constCreate()
{
    state = s_q0;
    createConst();
    register_name.clear();
}
void Lexer::constCreateAndBack()
{
    constCreate();
    --currentPos;
}
void Lexer::skipError()
{
    state = s_Err;
}
void Lexer::comma()
{
    state = s_q0;
    createComma();
}
void Lexer::q0()
{
    state = s_q0;
}
void Lexer::newLine()
{
    state = s_q0;
    ++currentLine;
}
void Lexer::semicolon()
{
    state = s_q0;
    createSemicolon();
}
void Lexer::comment()
{
    state = s_Comment;
}
void Lexer::commentNewLine()
{
    state = s_q0;
    ++currentLine;
}
void Lexer::commentCheckIfEnd()
{
    state = s_Comment;
    if (programm.c_str()[currentPos + 1] == ')')
    {
        state = s_q0;
        ++currentPos;
    }
}
void Lexer::CommentEnd1()
{
    if (currentSymbol == '>')
    {
        state = s_CommentEnd;
        return;
    }

    state = s_Comment;
}
void Lexer::CommentEnd2()
{
    if (currentSymbol == '>')
    {
        state = s_CommentEnd2;
        return;
    }
    state = s_Comment;
}
void Lexer::CommentEnd3()
{
    if (currentSymbol == '>')
    {
        state = s_q0;
        return;
    }
    state = s_Comment;
}
void Lexer::leftBracket()
{
    createLeftBracket();
    state = s_q0;
}
void Lexer::rightBracket()
{
    state = s_q0;
    createRightBracket();
}
void Lexer::leftSquareBracket()
{
    state = s_q0;
    createLeftSquareBracket();
}
void Lexer::rightSquareBracket()
{
    state = s_q0;
    createRightSquareBracket();
}
void Lexer::Rel1OperatorArrowIn()
{
    if (register_relation == "<" && currentSymbol == '-')
    {
        createOperatorArrowIn();
        register_relation.clear();
    }
    else
    {
        createOperator();
    }
    state = s_q0;
}
void Lexer::Op1()
{
    state = s_Op1;
    register_operator = currentSymbol;
}
void Lexer::OpMaybeCommentOrError()
{
    if (register_operator == '-' && currentSymbol == '-')
    {
        state = s_Comment;
    }
    else if (register_operator == '/' && currentSymbol == '/')
    {
        state = s_ConstType;
        createConstType(LexemType::ConstRgr1TypeBegin);
    }
    else
    {
        errorOperator();
    }
}
void Lexer::OpCreateAndBack()
{
    state = s_q0;
    createOperator();
    --currentPos;
}
void Lexer::OpifArrowOrBack()
{
    state = s_q0;
    if (register_operator == '-' && currentSymbol == '>')
    {
        createOperatorArrowOut();
    }
    else
    {
        createOperator();
    }
}
void Lexer::L1()
{
    state = s_L1;
    register_name.push_back(currentSymbol);
}
void Lexer::D1()
{
    state = s_D1;
    register_name.push_back(currentSymbol);
}
void Lexer::Rel1()
{
    state = s_Rel1;
    register_relation.push_back(currentSymbol);
}
void Lexer::RelCreateDouble()
{
    register_relation.push_back(currentSymbol);

    if (register_relation[0] == '<'  &&  currentSymbol == '<')
    {
        state = s_Rel2;
        return;
    }

    state = s_q0;
    bool isGood = relationTable[register_relation[0]][currentSymbol];
    if (!isGood)
    {
        errorRelation();
        return;
    }

    createRelation();
    register_relation.clear();
}
void Lexer::CommentCheck()
{
    if (currentSymbol == '<')
    {
        state = s_Comment;
        createComment();
        register_relation.clear();
        return;
    }

    errorRelation();
}
void Lexer::RelCreateSingle()
{
    state = s_q0;

    if (register_relation == "=")
        createAssignment();
    else 
        createRelation();

    register_relation.clear();
}
void Lexer::RelCreateAndBack()
{
    RelCreateSingle();
    --currentPos;
}
void Lexer::LCreate()
{
    auto keywordIt = std::find_if(keywords.begin(), keywords.end(), [this]
    (const Keyword& keyword_) { return keyword_.name == this->register_name; });
    if (keywordIt != keywords.end())
    {
        lexems.emplace_back(keywordIt->type, keywordIt->name);
    }
    else
    {
        lexems.emplace_back(LexemType::Identifier, register_name);
    }
    state = s_q0;
    register_name.clear();
}
void Lexer::LCreateAndBack()
{
    LCreate();
    --currentPos;
}
void Lexer::label()
{
    state = s_Label;
    register_name.push_back(currentSymbol);
}
void Lexer::labelCreate()
{
    state = s_q0;
    createLabel();
    register_name.clear();
}
void Lexer::labelCreateNewLine()
{
    labelCreate();
    ++currentLine;
}
void Lexer::doubleDot()
{
    char nextSym = programm.c_str()[currentPos + 1];
    if (isNewLine(nextSym) || isSpace(nextSym))
    {
        state = s_q0;
        createDoubleDot();
    }
    else
    {
        state = s_Label;
    }
}
void Lexer::constTypeValue()
{
    state = s_ConstType;
    register_name.push_back(currentSymbol);
}
void Lexer::constTypeCheckIfEndAndCreateConstIfNot()
{
    if (currentSymbol != '/')
    {
        errorConstType();
        return;
    }
    char nextSym = programm.c_str()[currentPos + 1];
    if (nextSym == '/')
    {
        state = s_q0;
        ++currentPos;
        createConst();
        register_name.clear();
        createConstType(LexemType::ConstRgr1TypeEnd);

        createConstString();
        return;
    }
    else
    {
        state = s_ConstType;
    }
    createConst();
    register_name.clear();
}
void Lexer::constTypeErrorNewLine()
{
    errorConstType();
    ++currentLine;
}
void Lexer::createConstString()
{
    std::string konstStr = "//";
    auto it = std::find_if(lexems.begin(), lexems.end(), [](const Lexem& lexem_)
        { return lexem_.GetLexemType() == LexemType::ConstRgr1TypeBegin; });
    ++it;
    while (it->GetLexemType() != LexemType::ConstRgr1TypeEnd)
    {
        konstStr += it->GetName();
        konstStr.push_back('/');
        ++it;
    }
    konstStr.push_back('/');
    removeLexemsConstType();
    register_name = konstStr;
    createConst();
    register_name.clear();
}