#ifndef _LEXER_H__
#define _LEXER_H__

#include <string>
#include <vector>
#include <map>
#include "tokens.h"
#include "lexems.h"

class Lexer
{
private:
    struct Keyword
    {
        Keyword(std::string name_, LexemType type_)
            : name(std::move(name_)), type(type_) {}

        std::string name;
        LexemType type;
    };
private:
    const std::vector<Keyword> keywords = {
        {"let", LexemType::Let},
        {"to", LexemType::To}, 
        {"jump", LexemType::Jump},
        {"while", LexemType::While},
        {"do", LexemType::Do},
        {"if", LexemType::If}, 
        {"else", LexemType::Else},
        {"end", LexemType::End},
        {"then", LexemType::Then},
        {"for", LexemType::For},
        {"abort", LexemType::Abort},
        {"otherwise", LexemType::Otherwise},
        {"difference", LexemType::Func},
        {"symmetric_difference", LexemType::Func},
        {"intersection", LexemType::Func}, 
        {"unification", LexemType::Func},
        {"int", LexemType::Type}, 
        {"boolMatrix", LexemType::Type}
    };
    std::map<char, std::map<char, bool>> relationTable = {
        {'=', {{'=', true}, {'>', false}, {'<', false}}},
        {'>', {{'=', true}, {'>', false}, {'<', false}}},
        {'<', {{'=', true}, {'>', false}, {'<', false}}},
    };
private:
    enum {
        s_q0, s_L1, s_D1, s_LCreate, s_Rel1, s_Rel2, s_EndRead, s_Err,
        s_Comment, s_Comment1, s_CommentEnd, s_CommentEnd2, s_Op1, s_Label,
        s_ConstType, StatesCount
    };
    using FuncPtr = void (Lexer::*)();
    int transliterator(char ch);
    bool isRelationSymbol(char);
    bool isSpace(char);
    bool isNewLine(char);
    bool isOperator(char);
    bool isEndOfFile(char);
    void initStates();
    void initErrorStates();

    void initD1State();
    void initq0State();
    void initL1State();
    void initRel1State();
    void initRel2State();
    void initCommentState();
    void initOp1State();
    void initLabelState();
    void initConstTypeState();

    void createOperator();
    void createRelation();
    void createOperatorArrowIn();
    void createOperatorArrowOut();
    void createSemicolon();
    void createLeftSquareBracket();
    void createRightSquareBracket();
    void createConst();
    void createLeftBracket();
    void createRightBracket();
    void createComma();
    void createAssignment();
    void createLabel();
    void createDoubleDot();
    void createConstType(LexemType type);
    void createConstString();
    void createComment();
    void CommentCheck();
    void CommentEnd1();
    void CommentEnd2();
    void CommentEnd3();
    void initCommentEnd();
    void initCommentEnd2();

    //Состояния
    void comma();
    void skipError();
    void q0();
    void D1();
    void doubleDot();

    //Нахождение идентификатора(или ключевого слова)
    void L1();
    void LCreate();
    void LCreateAndBack();

    //Нахождение оператора(+, -, * )
    void Op1();
    void OpCreateAndBack();
    void OpifArrowOrBack();
    void OpMaybeCommentOrError();

    //Нахождение отношения(>, <)
    void Rel1();
    void Rel1OperatorArrowIn();
    void RelCreateSingle();
    void RelCreateDouble();
    void RelCreateAndBack();

    void label();
    void labelCreate();
    void labelCreateNewLine();

    //Константа типа с ргр 1
    void slash();
    void constType();
    void constTypeValue();
    void constTypeCheckIfEndAndCreateConstIfNot();
    void constTypeErrorNewLine();

    void leftBracket();
    void rightBracket();
    //NL state
    void newLine();
    //Sem state
    void semicolon();
    //LSB state
    void leftSquareBracket();
    //RSB state
    void rightSquareBracket();

    void constCreate();
    void constCreateAndBack();
    void comment();
    void commentNewLine();
    void commentCheckIfEnd();

    //Обработка ошибок
    void errorUnexpectedSymbol();
    void errorConst();
    void errorRelation();
    void errorOperator();
    void errorLabelName();
    void errorLabelEnd();
    void errorConstType();
    void errroConstTypeContinue();

    void removeLexemsConstType();
public:
    Lexer(const std::string& programm_);
    std::vector<Lexem> Start();
private:
    FuncPtr states[StatesCount][TokensCount];
    int state;
    char currentSymbol;
    size_t currentPos = 0;
    size_t currentLine = 1;

    //Registers
    std::string register_name;
    std::string register_relation;
    std::string register_funcName;
    char register_operator;

    std::string programm;
    std::vector<Lexem> lexems;
};
#endif