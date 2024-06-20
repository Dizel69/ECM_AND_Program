#include <iostream>
#include <fstream>
#include <vector>
#include "hash.cpp"
#include "lexerrdef.h"

#if 1
    #define _PRINT_ERROR PRINT_ERROR()
#else
    #define _PRINT_ERROR    
#endif

class Lexer {
protected:
    Hash TS;        // таблица строк

    std::vector<float> TO;      // таблица операндов
    int NTO { 286 };
    int PTO;

    std::string identifier(int i) {
        char s1, s2 = '\0';
        s1 = i % 26 + 'A';
        if (i > 26) {
            s2 = i / 26 - 1 + '0';
        }

        char id[2];
        id[0] = s1;
        id[1] = s2;

        return id;
    }

    struct el_tl {
        int klass;
        int val;
    };

    void print_lexem(el_tl el, std::ostream& os = std::cout) {
        /*switch (el.klass) {
        case 1: os << "Метка " << el.val; break;
        case 2: os << "Операнд " << el.val; break;
        case 3: os << "Арифметическая опрерация ";
            switch (el.val) {
            case 1: os << '+'; break;
            case 2: os << '-'; break;
            case 3: os << '*'; break;
            case 4: os << '/'; break;
            case 5: os << '^'; break;
            } break;
        case 4: os << "Отношение ";
            switch (el.val) {
            case 1: os << '='; break;
            case 2: os << '<'; break;
            case 3: os << '>'; break;
            case 4: os << "<="; break;
            case 5: os << ">="; break;
            case 6: os << "<>"; break;
            } break;
        case 5: os << "NEXT " << el.val; break;
        case 6: os << "LET " << el.val; break;
        case 7: os << "FOR " << el.val; break;
        case 8: os << "GOTO " << el.val; break;
        case 9: os << "GOSUB " << el.val; break;
        case 10: os << "("; break;
        case 11: os << ")"; break;
        case 12: os << "IF"; break;
        case 13: os << "RETURN"; break;
        case 14: os << "END"; break;
        case 15: os << "TO"; break;
        case 16: os << "STEP"; break;
        case 17: os << "REM"; break;
        case 18: os << "Ошибка"; break;
        case 19: os << "EOF"; break;
        }*/
        os << el.klass << ' ' << el.val;
    }

    std::vector<el_tl> TL;      // таблица лексем
    int NTL{ 1 };
private:
    int RCH;    // регистр числа
    int RZ;     // регистр знака порядка
    int RP;     // регситр порядка
    int RS;     // регситр счётчика

    int RKL;    // регистр класса   

    int ROT;    // регистр отношения

    int RZN;    // регситр значения символа

    void (Lexer::* RSOS)() { &Lexer::A1 };   // регситр соcтояния

    int ROB;    // регистр обнаружения

    int RK;     // регистр значения класса символа

    int RSTR;   // регистр строки

    int VEC[26]{ 0, 0, 0, 0, 1, 3, 5, 0, 11, 0, 0, 12, 0, 14, 0, 0, 0, 17, 23, 26, 0, 0, 0, 0, 0, 0 };

    int RERR;
    const char* ESTR;

    struct el_dt {
        char letter;
        void (Lexer::* ptr)();
        int alt;
    };
    el_dt DT[27]{
        { '\0', nullptr, 0 },
        { 'N', &Lexer::C1b, 0 },
        { 'D', &Lexer::A2a, 0 },
        { 'O', &Lexer::C1b, 0 },
        { 'R', &Lexer::D1a, 0 },
        { 'O', &Lexer::C1b, 0 },
        { 'T', &Lexer::C1b, 8 },
        { 'O', &Lexer::E1a, 0 },
        { 'S', &Lexer::C1b, 0 },
        { 'U', &Lexer::C1b, 0 },
        { 'B', &Lexer::E1b, 0 },
        { 'F', &Lexer::A2b, 0 },
        { 'E', &Lexer::C1b, 0 },
        { 'T', &Lexer::D1b, 0 },
        { 'E', &Lexer::C1b, 0 },
        { 'X', &Lexer::C1b, 0 },
        { 'T', &Lexer::D1c, 0 },
        { 'E', &Lexer::C1b, 0 },
        { 'T', &Lexer::C1b, 22 },
        { 'U', &Lexer::C1b, 0 },
        { 'R', &Lexer::C1b, 0 },
        { 'N', &Lexer::A2c, 0 },
        { 'M', &Lexer::G1a, 0 },
        { 'T', &Lexer::C1b, 0 },
        { 'E', &Lexer::C1b, 0 },
        { 'P', &Lexer::A2d, 0 },
        { 'O', &Lexer::A2e, 0 },    
    };

    void translit(char ch) {
        if ('A' <= ch && ch <= 'Z') {
            RK = 1;
            RZN = ch - 'A';
        }
        else if ('a' <= ch && ch <= 'z') {
            RK = 1;
            RZN = ch - 'a';
        }
        else if ('0' <= ch && ch <= '9') {
            RK = 2;
            RZN = ch - '0';
        }
        else if (ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == '^')
        {
            RK = 3;
            switch (ch) {
            case '+': RZN = 1; break;
            case '-': RZN = 2; break;
            case '*': RZN = 3; break;
            case '/': RZN = 4; break;
            case '^': RZN = 5; break;
            }
        }
        else if (ch == '=' || ch == '<' || ch == '>') {
            RK = 4;
            switch (ch) {
            case '=': RZN = 1; break;
            case '<': RZN = 2; break;
            case '>': RZN = 3; break;
            }
        }
        else if (ch == '(') {
            RK = 5;
            RZN = 0;
        }
        else if (ch == ')') {
            RK = 6;
            RZN = 0;
        }
        else if (ch == '.') {
            RK = 7;
            RZN = 0;
        }
        else if (ch == ' ' || ch == '\t') {
            RK = 8;
            RZN = 0;
        }
        else if (ch == '\n') {
            RK = 9;
            RZN = 0;
        }
        else if (ch == EOF) {
            RK = 10;
            RZN = 0;
        }
        else {
            RK = 11;
            RZN = 0;
        }
    }

    void create_lexem() {
        switch (RKL) {
        case 1:  TS.Add(RSTR, -1); TL[NTL] = { RKL, RSTR }; break;
        case 2:  TL[NTL] = { RKL, PTO }; break;
        case 3:  TL[NTL] = { RKL, RZN }; break;
        case 4:  TL[NTL] = { RKL, ROT }; break;
        case 5:  TL[NTL] = { RKL, PTO }; break;
        case 6:  TL[NTL] = { RKL, PTO }; break;
        case 7:  TL[NTL] = { RKL, PTO }; break;
        case 8:  TS.Add(RCH, -1); TL[NTL] = { RKL, RCH }; break;
        case 9:  TS.Add(RCH, -1); TL[NTL] = { RKL, RCH }; break;
        default: TL[NTL] = { RKL, 0 };
        }
        if (RKL != 1 && RKL != 19) TS.Add(RSTR, NTL);
        NTL++;
    }

    void add_var() {
        TO[PTO] = 1;
    }

    void add_const() {
        float x = RCH;
        int exp = -RS + RZ * RP;
        if (exp > 0)
            for (int i = 0; i < exp; ++i)
                x *= 10;
        if (exp < 0)
            for (int i = 0; i < -exp; ++i)
                x /= 10;

        PTO = NTO;
        for (int i = 286; i < NTO; ++i) {
            if (TO[i] == x) {
                PTO = i;
            }
        }

        if (PTO == NTO) {
            TO[NTO] = x;
            NTO++;
        }
    }

    void A1a() {
        create_lexem();
        RSOS = &Lexer::A1;
    }

    void A2a() {
        RKL = 14;
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2b() {
        RKL = 12;
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2c() {
        RKL = 13;
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2d() {
        RKL = 16;
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2e() {
        RKL = 15;
        create_lexem();
        RSOS = &Lexer::A2;
    }
    
    void A2f() {
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2g() {
        RKL = 3;
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2h() {
        RKL = 10;
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2i() {
        RKL = 11;
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2j() {
        if (ROT == 2 && RZN == 1) ROT = 4;
        else if (ROT == 3 && RZN == 1) ROT = 5;
        else if (ROT == 2 && RZN == 3) ROT = 6;
        else { lex_error(INCORRECT_RELATION); return; }// wrong relation

        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2k() {
        if (RK == 2) PTO = PTO + 26 * (RZN + 1);
        add_var();
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2l() {
        PTO = NTO;
        add_const();
        create_lexem();
        RSOS = &Lexer::A2;
    }

    void A2m() {
        if (RKL == 5) {
            RKL = 4; ROT = 1;
            create_lexem();
            RSOS = &Lexer::A2;
            return;
        }
        if (RZN == 1)
            RSOS = &Lexer::A2;
        else
            lex_error(NO_ASSINGMENT);
    }

    void B1a() {
        RKL = 1;
        RSTR = RZN;
        RSOS = &Lexer::B1;
    }

    void B1b() {
        RSTR = 10 * RSTR + RZN;
        RSOS = &Lexer::B1;
    }

    void C1a() {
        create_lexem();

        ROB = VEC[RZN];
        if (ROB == 0) {
            lex_error(INCORRECT_KEYWORD); // error: wrong keyword
            return;
        }
        RSOS = &Lexer::C1;
    }

    void C1b() {
        ROB++;
        RSOS = &Lexer::C1;
    }

    void C2a() {
        RKL = 2;
        PTO = RZN;

        ROB = VEC[RZN];

        RSOS = &Lexer::C2;
    }

    void D1a() {
        RKL = 7;
        RSOS = &Lexer::D1;
    }

    void D1b() {
        RKL = 6;
        RSOS = &Lexer::D1;
    }

    void D1c() {
        RKL = 5;
        RSOS = &Lexer::D1;
    }

    void D2a() {
        PTO = RZN;
        RSOS = &Lexer::D2;
    }

    void E1a() {
        RKL = 8;
        RSOS = &Lexer::E1;
    }

    void E1b() {
        RKL = 9;
        RSOS = &Lexer::E1;
    }

    void E2a() {
        RCH = RZN;
        RSOS = &Lexer::E2;
    }

    void E2b() {
        RCH = 10 * RCH + RZN;
        RSOS = &Lexer::E2;
    }

    void F1a() {
        RKL = 4;
        ROT = RZN;
        RSOS = &Lexer::F1;
    }

    void G1a() {
        RKL = 17;
        create_lexem();
        RSOS = &Lexer::G1;
    }

    void H1a() {
        RKL = 2;
        RCH = RZN;
        RZ = 1;
        RP = RS = 0;
        RSOS = &Lexer::H1;
    }

    void H1b() {
        RCH = 10 * RCH + RZN;
        RSOS = &Lexer::H1;
    }

    void H2a() {
        RCH = 10 * RCH + RZN;
        RS++;
        RSOS = &Lexer::H2;
    }

    void H3a() {
        if (RZN != 'E' - 'A') {
            lex_error(INCORRECT_CONSTANT); // letter after number
            return;
        }
        RSOS = &Lexer::H3;
    }

    void H4a() {
        if (RZN != 1 && RZN != 2) {
            lex_error(INCORRECT_CONSTANT); // wrong const
            return;
        }

        if (RZN == 2) RZ = -RZ;

        RSOS = &Lexer::H4;
    }

    void H5a() {
        RP = RZN;
        RSOS = &Lexer::H5;
    }

    void H5b() {
        RP = 10 * RP + RZN;
        RSOS = &Lexer::H5;
    }

    void H6a() {
        RKL = 2;
        RCH = 0;
        RZ = 1;
        RP = RS = 0;
        RSOS = &Lexer::H6;
    }

    void M1() {
        while (true) {
            if (DT[ROB].letter - 'A' == RZN) {
                (this->*DT[ROB].ptr)();
                return;
            }

            ROB = DT[ROB].alt;

            if (ROB == 0) {
                lex_error(INCORRECT_KEYWORD); // wrong keyword
                return;
            }
        }
    }

    void EXIT1() {
        RKL = 19;
        create_lexem();
        RSOS = nullptr;
    }

    void lex_error(const char* err) {
        iswrong = true;

        RERR = string_buffer.size() - 1;
        ESTR = err;

        RKL = 18;
        create_lexem();
        RSOS = &Lexer::ERR;
    }

    void PRINT_ERROR() {
        std::cout << "error: " << ESTR << '\n';
        std::cout << "\t|  " << string_buffer;
        std::cout << "\t|  ";
        for (int i = 0; i < RERR; ++i) {
            std::cout << ' ';
        }
        std::cout << "^\n\n";
    }

    void A1() {
        switch (RK) {
        case 2: B1a(); return;
        case 8: return;
        case 9: return;
        case 10: EXIT1(); return;
        default: lex_error(LABEL_BEGIN); // error: label expected in the beginning of the line
        }
    }

    void A2() {
        switch (RK) {
        case 1: C2a(); return;
        case 2: H1a(); return;
        case 3: A2g(); return;
        case 4: F1a(); return;
        case 5: A2h(); return;
        case 6: A2i(); return;
        case 7: H6a(); return;
        case 8: return;
        case 9: RSOS = &Lexer::A1; return;
        case 10: EXIT1(); return;
        default: lex_error(INCORRECT_SYMBOL); // wrong symbol
        }
    }

    void B1() {
        switch (RK) {
        case 1: C1a(); return;
        case 2: B1b(); return;
        case 8: RSOS = &Lexer::B2; return;
        case 9: A1a(); return;
        case 10: A1a(); EXIT1(); return;
        default: lex_error(KEYWORD_AFTER_LABEL); // error: keyword after label expected
        }
    }
    
    void B2() {
        switch (RK) {
        case 1: C1a(); return;
        case 8: RSOS = &Lexer::B2; return;
        case 9: A1a(); return;
        case 10: A1a(); EXIT1(); return;
        default: lex_error(KEYWORD_AFTER_LABEL); // error: keyword after label expected
        }
    }

    void C1() {
        switch (RK) {
        case 1: M1(); return;
        case 9: lex_error(INCORRECT_KEYWORD); _PRINT_ERROR;  RSOS = &Lexer::A1; return;
        case 10: lex_error(INCORRECT_KEYWORD); _PRINT_ERROR;  EXIT1(); return;
        default: lex_error(INCORRECT_KEYWORD); // wrong keyword
        }
    }

    void C2() {
        switch (RK) {
        case 1: M1(); return;
        case 2: A2k(); return;
        case 3: create_lexem(); A2g(); return;
        case 4: create_lexem(); F1a(); return;
        case 5: create_lexem(); A2h(); return;
        case 6: create_lexem(); A2i(); return;
        case 7: create_lexem(); H6a(); return;
        case 8: A2f(); return;
        case 9: A1a(); return;
        case 10: create_lexem(); EXIT1(); return;
        default: lex_error(INCORRECT_SYMBOL); // wrong symbol
        }
    }

    void D1() {
        switch (RK) {
        case 1: D2a(); return;
        case 8: return;
        case 9: lex_error(VAR_EXPECTED); _PRINT_ERROR; RSOS = &Lexer::A1; return;
        case 10: lex_error(VAR_EXPECTED); _PRINT_ERROR;  EXIT1(); return;
        default: lex_error(VAR_EXPECTED); // no variable after for/let/next
        }
    }

    void D2() {
        switch (RK) {
        case 2: A2k(); RSOS = &Lexer::I1; return;
        case 4: A2k(); A2m(); return;
        case 8: add_var(); A2f(); RSOS = &Lexer::I1; return;
        case 9: if (RKL != 5) { lex_error(NO_ASSINGMENT); _PRINT_ERROR; }
                else { create_lexem(); }
                RSOS = &Lexer::A1; return;
        case 10: if (RKL != 5) { lex_error(NO_ASSINGMENT); _PRINT_ERROR; }
                 else { create_lexem(); }
                 EXIT1(); return;
        default: lex_error(VAR_EXPECTED);
        }
    }

    void I1() {
        switch (RK) {
        case 4: A2m(); return;
        case 8: return;
        case 9: lex_error(NO_ASSINGMENT); _PRINT_ERROR; RSOS = &Lexer::A1; return;
        case 10: lex_error(NO_ASSINGMENT); _PRINT_ERROR;  EXIT1(); return;
        default: lex_error(NO_ASSINGMENT);
        }
    }

    void E1() {
        switch (RK) {
        case 2: E2a(); return;
        case 8: return;
        case 9: lex_error(LABEL_EXPECTED); _PRINT_ERROR;  RSOS = &Lexer::A1; return;
        case 10: lex_error(LABEL_EXPECTED); _PRINT_ERROR;  EXIT1(); return;
        default: lex_error(LABEL_EXPECTED); // no label after goto/gosub
        }
    }

    void E2() {
        switch (RK) {
        case 1: C2a(); return;
        case 2: E2b(); return;
        case 3: create_lexem(); A2g(); return;
        case 4: create_lexem(); F1a(); return;
        case 5: create_lexem(); A2h(); return;
        case 6: create_lexem(); A2i(); return;
        case 8: A2f(); return;
        case 9: A1a(); return;
        case 10: A1a(); EXIT1(); return;
        default: lex_error(INCORRECT_SYMBOL); // wrong symbol
        }
    }

    void F1() {
        switch (RK) {
        case 1: create_lexem(); C2a(); return;
        case 2: create_lexem(); H1a(); return;
        case 3: lex_error(INCORRECT_RELATION); return; // worng relation
        case 4: A2j(); return;
        case 5: create_lexem(); A2h(); return;
        case 6: create_lexem(); A2i(); return;
        case 7: create_lexem(); H6a(); return;
        case 8: A2f(); return;
        case 9: A1a(); return;
        case 10: create_lexem(); EXIT1(); return;
        default: lex_error(INCORRECT_SYMBOL); // wrong symbol
        }
    }

    void G1() {
        switch (RK) {
        case 9: RSOS = &Lexer::A1; return;
        case 10: EXIT1(); return;
        default: return;
        }
    }

    void H1() {
        switch (RK) {
        case 1: H3a(); return;
        case 2: H1b(); return;
        case 3: A2l(); A2g(); return;
        case 4: A2l(); F1a(); return;
        case 5: A2l(); A2h(); return;
        case 6: A2l(); A2i(); return;
        case 7: RSOS = &Lexer::H2; return;
        case 8: A2l(); return;
        case 9: A2l(); RSOS = &Lexer::A1; return;
        case 10: A2l(); EXIT1(); return;
        default: lex_error(INCORRECT_CONSTANT); // wrong const
        }
    }

    void H2() {
        switch (RK) {
        case 1: H3a(); return;
        case 2: H2a(); return;
        case 3: A2l(); A2g(); return;
        case 4: A2l(); F1a(); return;
        case 5: A2l(); A2h(); return;
        case 6: A2l(); A2i(); return;
        case 8: A2l(); return;
        case 9: A2l(); RSOS = &Lexer::A1; return;
        case 10: A2l(); EXIT1(); return;
        default: lex_error(INCORRECT_CONSTANT); // wrong const
        }
    }

    void H3() {
        switch (RK) {
        case 2: H5a(); return;
        case 3: H4a(); return;
        case 9: lex_error(INCORRECT_CONSTANT); _PRINT_ERROR;  RSOS = &Lexer::A1; return;
        case 10: lex_error(INCORRECT_CONSTANT); _PRINT_ERROR;  EXIT1(); return;
        default: lex_error(INCORRECT_CONSTANT); // wrong const
        }
    }

    void H4() {
        switch (RK) {
        case 2: H5a(); return;
        case 9: lex_error(INCORRECT_CONSTANT); _PRINT_ERROR;  RSOS = &Lexer::A1; return;
        case 10: lex_error(INCORRECT_CONSTANT); _PRINT_ERROR;  EXIT1(); return;
        default: lex_error(INCORRECT_CONSTANT); // wrong const
        }
    }

    void H5() {
        switch (RK) {
        case 2: H5b(); return;
        case 3: A2l(); A2g(); return;
        case 4: A2l(); F1a(); return;
        case 5: A2l(); A2h(); return;
        case 6: A2l(); A2i(); return;
        case 8: A2l(); return;
        case 9: A2l(); RSOS = &Lexer::A1; return;
        case 10: A2l(); EXIT1(); return;
        default: lex_error(INCORRECT_CONSTANT); // wrong const
        }
    }

    void H6() {
        switch (RK) {
        case 2: H2a(); return;
        case 9: lex_error(INCORRECT_CONSTANT); _PRINT_ERROR;  RSOS = &Lexer::A1; return;
        case 10: lex_error(INCORRECT_CONSTANT); _PRINT_ERROR;  EXIT1(); return;
        default: lex_error(INCORRECT_CONSTANT); // wrong const
        }
    }

    void ERR() {
        switch (RK) {
        case 9: _PRINT_ERROR; RSOS = &Lexer::A1; return;
        case 10: _PRINT_ERROR; EXIT1(); return;
        default: return;
        }
    }

    std::string string_buffer;
public:
    bool iswrong { false };

    Lexer() : TO(640), TL(2014) {
        //std::cout << identifier(44) << " " << identifier(70) << '\n';
    }

    void run_lexer(const std::string& path) {
        std::ifstream file(path);

        if (!file) {
            std::cout << "не удалось открыть " << path << std::endl;
            exit(1);
        }

        while (true) {
            char x = file.get();

            if (x == '\t') string_buffer += ' ';
            else if (x == EOF) string_buffer += '\n';
            else string_buffer += x;

            translit(x);

            (this->*RSOS)();

            if (x == '\n' || x == EOF) {
                string_buffer.clear();
            }

            if (RSOS == nullptr) break;
        }
    }

    void print_lexer_info(std::ostream& os = std::cout) {
        os << "TS:\n";
        TS.Print_t(os);

        os << "\n\nTL:\n";
        for (int i = 1; i < NTL; ++i) {
            os << i << ". ";
            print_lexem(TL[i], os);
            os << '\n';
        }

        os << "\n\nTO:\n";
        os << "var:\n";
        for (int i = 0; i < 286; ++i) {
            if (TO[i] != 0) os << i << ". " << identifier(i) <<  '\n';
        }
        os << "const:\n";
        for (int i = 286; i < NTO; ++i) {
            os << i << ". " << TO[i] << '\n';
        }
    }
};

#if 1

#else

int main() {
    Hash H(10);

    H.Add(4, 2);
    H.Add(14, 2);
    H.Add(24, 2);

    H.Add(5, 2);
    H.Add(25, 2);

    H.Print_t();
    std::cout << "\n\n";

    H.Del(14);
    H.Add(34, 2);

    H.Print_t();
}

#endif
