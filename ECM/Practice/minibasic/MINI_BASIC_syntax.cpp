#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <windows.h>
#include "MINI_BASIC_lexer.cpp"

class Syntax : public Lexer {
protected:
    struct Atom {
        int klass;
        int attributes[4];
    };
    std::vector<Atom> TA;
    int NTA { 0 };

    int ITL { 1 };

    int NS{ 0 };

    int NOVT{ NTO };
    int NOVTAM{ 1000 };

private:
    typedef void (Syntax::*proc_ptr)();

    proc_ptr PMX[33][23];

    int lexem_to_column(el_tl eltl) {
        switch (eltl.klass) {
        case 1: return 0;
        case 2: return 1;
        case 3: switch (eltl.val) {
                case 1: return 16;
                case 2: return 17;
                case 3: return 18;
                case 4: return 19;
                case 5: return 20;
                }
        case 18: return 21;
        case 19: return 22;
        default: return eltl.klass - 2;
        }
        /*switch (eltl.klass) {
        case 1: return 0;
        case 2: return 1;
        case 3: switch (eltl.val) {
                case 1: return 16;
                case 2: return 17;
                case 3: return 18;
                case 4: return 19;
                case 5: return 20;
                }
        case 4: return 2;
        case 5: return 3;
        case 6: return 4;
        case 7: return 5;
        case 8: return 6;
        case 9: return 7;
        case 10: return 8;
        case 11: return 9;
        case 12: return 10;
        case 13: return 11;
        case 14: return 12;
        case 15: return 13;
        case 16: return 14;
        case 17: return 15;
        case 18: return 21;
        case 19: return 22;
        }*/
    }

    std::deque<int> Stack;

    void Push() {}

    template<typename... Types>
    void Push(int x, Types... args) {
        Push(args...);
        Stack.push_back(x);
    }

    void Pop() {
        Stack.pop_back();
    }

    void rule1() {
        Pop();

        Push(1, 15);
    }

    void rule3() {
        Pop();

        Push(1);
    }

    void rule4() {
        Pop();

        int p = TL[ITL].val;

        Push(4, 3,
             18, p, -1,
             3);
    }

    void rule57812() {
        Pop();

        Push(3);
    }

    void rule6() {
        Pop();

        Push(4, 8,
            11, 8,
            4, 5,
            13, 5,
            19, -1, -1, -1, -1,
            3);
    }

    void rule9() {
        Pop();

        int p = TL[ITL].val;
        int s = NOVT++;
        int t = NOVT++;
        int u = NOVT++;
        int v = NOVT++;
        int y = NS;

        Push(4, 3,
             18, p, -1,
             16,
             4, 2,
             20, -1, s,
             2, 2,
             20, -1, t,
             21, u,
             22, p, s, t, v,
             3,
             12, 3,
             23, p, -1, y,
             24, p, t,
             25, u,
             21, v,
             3);
    }

    void rule10() {
        Pop();
        int x = Stack.back();
        Pop();

        Push(4, x);
    }

    void rule13() {
        Pop();

        Push(1);
    }

    void rule14() {
        Pop();
        int t = Stack.back();
        Pop();

        Push(5, 2,
             8, -1, t);
    }

    void rule15() {
        Pop();
        int t = Stack.back();
        Pop();

        int r = NOVT++;
        
        Push(5, 2,
             31, -1, r,
             8, r, t);
    }

    void rule16() {
        Pop();
        int t = Stack.back();
        Pop();

        int r = NOVT++;

        Push(5, 2,
             32, -1, r,
             8, r, t);
    }

    void rule17() {
        Pop();
        int p = Stack.back();
        Pop();
        int t = Stack.back();
        Pop();

        int r = NOVT++;

        Push(5, 3,
             26, p, -1, r,
             8, r, t);
    }

    void rule18() {
        Pop();
        int p = Stack.back();
        Pop();
        int t = Stack.back();
        Pop();

        int r = NOVT++;

        Push(5, 3,
             27, p, -1, r,
             8, r, t);
    }

    void rule20() {
        Pop();
        int t = Stack.back();
        Pop();


        Push(6, 2,
             9, -1, t);
    }

    void rule21() {
        Pop();
        int p = Stack.back();
        Pop();
        int t = Stack.back();
        Pop();

        int r = NOVT++;

        Push(6, 3,
             28, p, -1, r,
             9, r, t);
    }

    void rule22() {
        Pop();
        int p = Stack.back();
        Pop();
        int t = Stack.back();
        Pop();

        int r = NOVT++;

        Push(6, 3,
             29, p, -1, r,
             9, r, t);
    }

    void rule24() {
        Pop();
        int t = Stack.back();
        Pop();

        Push(7, 2,
             10, -1, t);
    }

    void rule25() {
        Pop();
        int p = Stack.back();
        Pop();
        int t = Stack.back();
        Pop();

        int r = NOVT++;

        Push(7, 3,
             30, p, -1, t,
             10, r, t);
    }

    void rule27() {
        Pop();
        int p = Stack.back();
        Pop();

        Push(4, p,
             14);
    }

    void new_atom(Atom atom) {
        TA[NTA] = atom;
        NTA++;
    }

    void proc1() {
        rule1();

        int p = TL[ITL].val;
        new_atom({ 2, {p, -1, -1, -1} });
        NS = p;

        ITL++;
    }

    void proc2() {
        Pop();
    }

    void proc3() {
        rule3();

        int p = TL[ITL].val;
        new_atom({ 2, {p, -1, -1, -1} });
        NS = p;

        ITL++;
    }

    void proc4() {
        rule4();
        ITL++;
    }

    void proc5() {
        rule57812();

        int p = TL[ITL].val;
        new_atom({ 4, {p, -1, -1, -1} });
        ITL++;
    }

    void proc6() {
        rule6();
        ITL++;
    }

    void proc7() {
        rule57812();

        int p = TL[ITL].val;
        new_atom({ 5, {p, -1, -1, -1} });
        ITL++;
    }

    void proc8() {
        rule57812();

        new_atom({ 6, {-1, -1, -1, -1} });
        ITL++;
    }

    void proc9() {
        rule9();
        ITL++;
    }

    void proc10() {
        rule10();
        ITL++;
    }

    void send(int data, int offset) {
        Stack[Stack.size() - offset] = data;
    }

    void proc11() {
        int i1 = NTO;
        for (int i = 286; i < NTO; ++i) {
            if (TO[i] == 1) {
                i1 = i;
                break;
            }
        }
        if (i1 == NTO) {
            TO[NTO] = 1;
            NTO++;
        }

        Pop();
        int offset = Stack.back();
        Pop();

        send(i1, offset);
    }

    void proc12() {
        rule57812();
        ITL++;
    }

    void proc13() {
        rule13();

        int p = TL[ITL].val;
        new_atom({ 2, {p, -1, -1, -1} });
        NS = p;
        ITL++;
    }

    void proc14() {
        rule14();
    }

    void proc15() {
        rule15();
        ITL++;
    }

    void proc16() {
        rule16();
        ITL++;
    }

    void proc17() {
        rule17();
        ITL++;
    }

    void proc18() {
        rule18();
        ITL++;
    }

    void proc19() {
        Pop();
        int x = Stack.back();
        Pop();
        int offset = Stack.back();
        Pop();

        send(x, offset);
    }

    void proc20() {
        rule20();
    }

    void proc21() {
        rule21();
        ITL++;
    }

    void proc22() {
        rule22();
        ITL++;
    }

    void proc23() {
        Pop();
        int x = Stack.back();
        Pop();
        int offset = Stack.back();
        Pop();

        send(x, offset);
    }

    void proc24() {
        rule24();
    }

    void proc25() {
        rule25();
        ITL++;
    }

    void proc26() {
        Pop();
        int x = Stack.back();
        Pop();
        int offset = Stack.back();
        Pop();

        send(x, offset);
    }

    void proc27() {
        rule27();
        ITL++;
    }

    void proc28() {
        Pop();
        int offset = Stack.back();
        Pop();


        int x = TL[ITL].val;
        send(x, offset);

        ITL++;
    }

    void a() {
        Pop();
        ITL++;
    }

    void b() {
        Pop();
        int offset = Stack.back();
        Pop();

        int x = TL[ITL].val;

        send(x, offset);

        ITL++;
    }

    void c() {
        new_atom({ 1, {-1, -1, -1, -1} });
        ITL++;
    }

    void d() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();

        new_atom({ 3, {p, q, -1, -1} });
    }

    void e() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();
        int r = Stack.back();
        Pop();
        int s = Stack.back();
        Pop();

        new_atom({ 7, {p, q, r, s} });
    }

    void f() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();

        new_atom({ 8, {p, q, -1, -1} });
    }

    void g() {
        Pop();
        int p = Stack.back();
        Pop();

        new_atom({ 9, {p, -1, -1, -1} });
    }

    void h() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();
        int r = Stack.back();
        Pop();
        int s = Stack.back();
        Pop();

        new_atom({ 10, {p, q, r, s} });
    }

    void i() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();
        int y = Stack.back();
        Pop();

        if (p != q) {
            std::cout << "ПЕРЕМЕННАЯ NEXT - ОПЕРАТОРА ОТЛИЧНА ОТ ПЕРЕМЕННОЙ FOR - ОПЕРАТОРА В СТРОКЕ " << y << "\n"
                      << identifier(TL[TS.Find(y)].val) << " - ПРЕДПОЛАГАЕМАЯ ПЕРЕМЕННАЯ\n";
            iswrong = true;
        }
    }

    void j() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();

        new_atom({ 11, {p, q, -1, -1} });
    }

    void k() {
        Pop();
        int p = Stack.back();
        Pop();

        new_atom({ 4, {p, -1, -1, -1} });
    }

    void l() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();
        int r = Stack.back();
        Pop();

        new_atom({ 12, {p, q, r, -1} });
    }

    void m() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();
        int r = Stack.back();
        Pop();

        new_atom({ 13, {p, q, r, -1} });
    }

    void n() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();
        int r = Stack.back();
        Pop();

        new_atom({ 14, {p, q, r, -1} });
    }

    void o() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();
        int r = Stack.back();
        Pop();

        new_atom({ 15, {p, q, r, -1} });
    }

    void p() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();
        int r = Stack.back();
        Pop();

        new_atom({ 16, {p, q, r, -1} });
    }

    void q() {
        Pop();
        int p = Stack.back();
        Pop();
        int q = Stack.back();
        Pop();

        new_atom({ 17, {p, q, -1, -1} });
    }

    void r() {
        Pop();
        int p = Stack.back();       
        Pop();
        int q = Stack.back();
        Pop();

        new_atom({ 18, {p, q, -1, -1} });
    }

    void synt_error(const std::string& msg) {
        std::cout << msg;
        iswrong = true;
    }

    void A() {
        synt_error("ПРОГРАММА НАЧИНАЕТСЯ НЕВЕРНО\n");
    }
    void B() {
        synt_error("ОПЕРАТОР НАЧИНАЕТСЯ НЕВЕРНО\n");
    }
    void G1() {
        synt_error("НЕОЖИДАННЫЙ \"");
        print_lexem(TL[ITL]);
        std::cout << "\" В КОНЦЕ FOR - ОПЕРАТОРА\n";
    }
    void G2() {
        synt_error("ЛИШНЯЯ ПРАВАЯ СКОБКА В ВЫРАЖЕНИИ ПОСЛЕ TO\n");
    }
    void D() {
        synt_error("НЕОЖИДАННЫЙ \"");
        print_lexem(TL[ITL]);
        std::cout << "\" ПОСЛЕ ОКОНЧАНИЯ ОПЕРАТОРА\n";
    }
    void K1() {
        synt_error("ВЫРАЖЕНИЕ НЕЗАКОННО - НЕТ ОПЕРАНДА\n");
    }
    void K2() {
        synt_error("ОПЕРАТОР НЕЗАКОНЧЕН\n");
    }
    void K3() {
        synt_error("В ВЫРАЖЕНИИ ПРОПУЩЕН ОПЕРАНД\n");
    }
    void K4() {
        synt_error("НЕОЖИДАННЫЙ \"");
        print_lexem(TL[ITL]);
        std::cout << "\" ПОСЛЕ ВЫРАЖЕНИЯ\n";
    }
    void K5() {
        synt_error("ВЫРАЖЕНИЕ НАЧИНАЕТСЯ С \"");
        print_lexem(TL[ITL]);
        std::cout << "\"\n";
    }
    void K6() {
        synt_error("ДВА ЗНАКА ОПЕРАЦИИ ПОДРЯД В ВЫРАЖЕНИИ\n");
    }
    void K7() {
        synt_error("В ВЫРАЖЕНИИ ПРОПУЩЕН ЗНАК ОПЕРАЦИИ\n");
    }
    void E1() {
        synt_error("НЕОЖИДАННЫЙ \"");
        print_lexem(TL[ITL]);
        std::cout << "\" В IF-ОПЕРАТОРА\n";
    }
    void E2() {
        synt_error("IF-ОПЕРАТОР НЕЗАКОНЧЕН\n");
    }
    void E3() {
        synt_error("ЛИШНЯЯ ПРАВАЯ СКОБКА В ВЫРАЖЕНИИ ПОСЛЕ IF\n");
    }
    void L() {
        synt_error("FOR-ОПЕРАТОРЫ ВЛОЖЕНЫ НЕПРАВИЛЬНО ПРОПУЩЕНО NEXT\n");
    }
    void M1() {
        synt_error("НЕОЖИДАННЫЙ \"");
        print_lexem(TL[ITL]);
        std::cout << "\" В IF-ОПЕРАТОРЕ\n";
    }
    void M2() {
        synt_error("IF-ОПЕРАТОР НЕЗАКОНЧЕН\n");
    }
    void M3() {
        synt_error("ДВА ЗНАКА ОТНОШЕНИЯ  В IF-ОПЕРАТОРЕ\n");
    }
    void M4() {
        synt_error("ЛИШНЯЯ ПРАВАЯ СКОБКА В ВЫРАЖЕНИИ ПОСЛЕ ЗНАКА ОТНОШЕНИЯ\n");
    }
    void H1() {
        synt_error("ПРОПУЩЕНА ПРАВАЯ СКОБКА ПЕРЕД \"");
        print_lexem(TL[ITL]);
        std::cout << "\"\n";
    }
    void H2() {
        synt_error("ПРОПУЩЕНА ПРАВАЯ СКОБКА В КОНЦЕ СТРОКИ\n");
    }
    void K() {
        synt_error("FOR-ОПЕРАТОРЫ ВЛОЖЕНЫ НЕПРАВИЛЬНО - ЛИШНЕЕ NEXT\n");
    }
    void P1() {
        synt_error("НЕОЖИДАННЫЙ \"");
        print_lexem(TL[ITL]);
        std::cout << "\" В FOR-ОПЕРАТОРЕ\n";
    }
    void P2() {
        synt_error("FOR-ОПЕРАТОР НЕЗАКОНЧЕН\n");
    }
    void P3() {
        synt_error("ЛИШНЯЯ ПРАВАЯ СКОБКА В ВЫРАЖЕНИИ ПОСЛЕ-\n");
    }
    void P4() {
        synt_error("TO ПРОПУЩЕНО ИЛИ НЕ НА СВОЁМ МЕСТЕ В FOR-ОПЕРАТОРЕ\n");
    }
    void R() {
        synt_error("ПРОГРАММА ПРОДОЛЖАЕТСЯ ПОСЛЕ END-ОПЕРАТОРА\n");
    }
    void C() {
        synt_error("ОШИБКА \n");
    }
    void T1() {
        synt_error("НЕТ ПРОГРАММЫ\n");
    }
    void T2() {
        synt_error("ПРОПУЩЕН END-ОПЕРАТОР\n");
    }
    void T3() {
        synt_error("ПРОГРАММА КОНЧАЕТСЯ ПОСРЕДИ ОПЕРАТОРА\n");
    }
    void O() {
        synt_error("ОШИБКА КОМПИЛЯТОРА\n");
    }
public:
    Syntax() : TA(1024) {
        // 0 row
        for (int i = 0; i < 23; ++i) {
            PMX[0][i] = &Syntax::A;
        }
        PMX[0][0] = &Syntax::proc1;
        PMX[0][21] = &Syntax::C;
        PMX[0][22] = &Syntax::T1;
        // 1 row
        for (int i = 0; i < 23; ++i) {
            PMX[1][i] = &Syntax::B;
        }
        PMX[1][0] = &Syntax::proc3;
        PMX[1][3] = PMX[1][12] = &Syntax::proc2;
        PMX[1][4] = &Syntax::proc4;
        PMX[1][5] = &Syntax::proc9;
        PMX[1][6] = &Syntax::proc5;
        PMX[1][7] = &Syntax::proc7;
        PMX[1][10] = &Syntax::proc6;
        PMX[1][11] = &Syntax::proc8;
        PMX[1][15] = &Syntax::proc12;
        PMX[1][21] = &Syntax::C;
        PMX[1][22] = &Syntax::T2;
        // 2 row
        for (int i = 0; i < 23; ++i) {
            PMX[2][i] = &Syntax::G1;
        }
        PMX[2][0] = &Syntax::proc11;
        PMX[2][9] = &Syntax::G2;
        PMX[2][14] = &Syntax::proc10;
        PMX[2][21] = &Syntax::C;
        PMX[2][22] = &Syntax::T2;
        // 3 row
        for (int i = 0; i < 23; ++i) {
            PMX[3][i] = &Syntax::D;
        }
        PMX[3][0] = &Syntax::proc13;
        PMX[3][21] = &Syntax::C;
        PMX[3][22] = &Syntax::T2;
        // 4 row
        for (int i = 0; i < 23; ++i) {
            PMX[4][i] = &Syntax::K4;
        }
        PMX[4][0] = &Syntax::K2;
        PMX[4][1] = PMX[4][8] = &Syntax::proc14;
        PMX[4][2] = PMX[4][6] = PMX[4][13] = PMX[4][14] = &Syntax::K1;
        PMX[4][9] = &Syntax::K3;
        PMX[4][16] = &Syntax::proc15;
        PMX[4][17] = &Syntax::proc16;
        PMX[4][18] = PMX[4][19] = PMX[4][20] = &Syntax::K5;
        PMX[4][21] = &Syntax::C;
        PMX[4][22] = &Syntax::T3;
        // 5 row
        for (int i = 0; i < 23; ++i) {
            PMX[5][i] = &Syntax::K4;
        }
        PMX[5][0] = &Syntax::K2;
        PMX[5][1] = PMX[5][8] = &Syntax::proc20;
        PMX[5][2] = PMX[5][6] = PMX[5][13] = PMX[5][14] = &Syntax::K1;
        PMX[5][9] = &Syntax::K3;
        PMX[5][16] = PMX[5][17] = PMX[5][18] = PMX[5][19] = PMX[5][20] = &Syntax::K6;
        PMX[5][21] = &Syntax::C;
        PMX[5][22] = &Syntax::T3;
        // 6 row
        for (int i = 0; i < 23; ++i) {
            PMX[6][i] = &Syntax::K4;
        }
        PMX[6][0] = &Syntax::K2;
        PMX[6][1] = PMX[6][8] = &Syntax::proc24;
        PMX[6][2] = PMX[6][6] = PMX[6][13] = PMX[6][14] = &Syntax::K1;
        PMX[6][9] = &Syntax::K3;
        PMX[6][16] = PMX[6][17] = PMX[6][18] = PMX[6][19] = PMX[6][20] = &Syntax::K6;
        PMX[6][21] = &Syntax::C;
        PMX[6][22] = &Syntax::T3;
        // 7 row
        for (int i = 0; i < 23; ++i) {
            PMX[7][i] = &Syntax::K4;
        }
        PMX[7][0] = &Syntax::K2;
        PMX[7][1] = &Syntax::proc28;
        PMX[7][2] = PMX[7][6] = PMX[7][13] = PMX[7][14] = &Syntax::K1;
        PMX[7][8] = &Syntax::proc27;
        PMX[7][9] = &Syntax::K3;
        PMX[7][16] = PMX[7][17] = PMX[7][18] = PMX[7][19] = PMX[7][20] = &Syntax::K6;
        PMX[7][21] = &Syntax::C;
        PMX[7][22] = &Syntax::T3;
        // 8 row
        for (int i = 0; i < 23; ++i) {
            PMX[8][i] = &Syntax::O;
        }
        PMX[8][0] = PMX[8][2] = PMX[8][6] = PMX[8][9] = PMX[8][13] = PMX[8][14] = &Syntax::proc19;
        PMX[8][1] = PMX[8][8] = &Syntax::K7;
        PMX[8][16] = &Syntax::proc17;
        PMX[8][17] = &Syntax::proc18;
        // 9 row
        for (int i = 0; i < 23; ++i) {
            PMX[9][i] = &Syntax::O;
        }
        PMX[9][0] = PMX[9][2] = PMX[9][6] = PMX[9][9] = PMX[9][13] = PMX[9][14] = PMX[9][16] = PMX[9][17] = &Syntax::proc23;
        PMX[9][1] = PMX[9][8] = &Syntax::K7;
        PMX[9][18] = &Syntax::proc21;
        PMX[9][19] = &Syntax::proc22;
        // 10 row
        for (int i = 0; i < 23; ++i) {
            PMX[10][i] = &Syntax::proc26;
        }
        PMX[10][1] = PMX[10][8] = &Syntax::K7;
        PMX[10][3] = PMX[10][4] = PMX[10][5] = PMX[10][7] = PMX[10][10] = PMX[10][11] = PMX[10][12] = PMX[10][15] = &Syntax::K4;
        PMX[10][20] = &Syntax::proc25;
        PMX[10][21] = &Syntax::C;
        PMX[10][22] = &Syntax::T2;
        // 11 row
        for (int i = 0; i < 23; ++i) {
            PMX[11][i] = &Syntax::E1;
        }
        PMX[11][0] = &Syntax::E2;
        PMX[11][2] = &Syntax::b;
        PMX[11][9] = &Syntax::E3;
        PMX[11][21] = &Syntax::C;
        PMX[11][22] = &Syntax::T3;
        // 12 row
        for (int i = 0; i < 23; ++i) {
            PMX[12][i] = &Syntax::O;
        }
        PMX[12][3] = &Syntax::b;
        PMX[12][12] = &Syntax::L;
        // 13 row
        for (int i = 0; i < 23; ++i) {
            PMX[13][i] = &Syntax::M1;
        }
        PMX[13][0] = &Syntax::M2;
        PMX[13][2] = &Syntax::M3;
        PMX[13][6] = &Syntax::b;
        PMX[13][9] = &Syntax::M4;
        PMX[13][21] = &Syntax::C;
        PMX[13][22] = &Syntax::T3;
        // 14 row
        for (int i = 0; i < 23; ++i) {
            PMX[14][i] = &Syntax::H1;
        }
        PMX[14][0] = &Syntax::H2;
        PMX[14][9] = &Syntax::a;
        PMX[14][21] = &Syntax::C;
        PMX[14][22] = &Syntax::T3;
        // 15 row
        for (int i = 0; i < 23; ++i) {
            PMX[15][i] = &Syntax::O;
        }
        PMX[15][3] = &Syntax::K;
        PMX[15][12] = &Syntax::a;
        // 16 row
        for (int i = 0; i < 23; ++i) {
            PMX[16][i] = &Syntax::P1;
        }
        PMX[16][0] = &Syntax::P2;
        PMX[16][9] = &Syntax::P3;
        PMX[16][13] = &Syntax::a;
        PMX[16][14] = &Syntax::P4;
        PMX[16][21] = &Syntax::C;
        PMX[16][22] = &Syntax::T3;
        // 17 row
        for (int i = 0; i < 23; ++i) {
            PMX[17][i] = &Syntax::R;
        }
        PMX[17][21] = &Syntax::C;
        PMX[17][22] = &Syntax::c;
        // 18 - 32 row
        for (int i = 0; i < 23; ++i) {
            PMX[18][i] = &Syntax::d;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[19][i] = &Syntax::e;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[20][i] = &Syntax::f;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[21][i] = &Syntax::g;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[22][i] = &Syntax::h;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[23][i] = &Syntax::i;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[24][i] = &Syntax::j;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[25][i] = &Syntax::k;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[26][i] = &Syntax::l;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[27][i] = &Syntax::m;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[28][i] = &Syntax::n;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[29][i] = &Syntax::o;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[30][i] = &Syntax::p;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[31][i] = &Syntax::q;
        }
        for (int i = 0; i < 23; ++i) {
            PMX[32][i] = &Syntax::r;
        }
    }

    void run_syntax() {
        Push(0, 17);

        while (ITL < NTL) {
            (this->*PMX[Stack.back()][lexem_to_column(TL[ITL])])();

            if (iswrong)
                break;
        }
    }

    void print_attr(Atom atom, int k, std::ostream& os = std::cout) {
        os << '{';
        for (int i = 0; i < k; ++i) {
            os << atom.attributes[i];
            if (i != k - 1) os << ' ';
        }
        os << '}';
    }

    void print_atom(Atom atom, std::ostream& os = std::cout) {
        /*switch (atom.klass) {
        case 1: os << "ОКНОЧАНИЕ"; return;
        case 2: os << "НОМСТРОК "; print_attr(atom, 1, os); return;
        case 3: os << "ПРИСВ "; print_attr(atom, 2, os); return;
        case 4: os << "ПЕРЕХОД "; print_attr(atom, 1, os); return;
        case 5: os << "ХРАНПЕРЕХОД "; print_attr(atom, 1, os); return;
        case 6: os << "ВОЗВПЕРЕХОД"; return;
        case 7: os << "УСЛ. ПЕРЕХОД "; print_attr(atom, 4, os); return;
        case 8: os << "ХРАНЕНИЕ "; print_attr(atom, 2, os); return;
        case 9: os << "МЕТКА "; print_attr(atom, 1, os); return;
        case 10: os << "ПРОВЕРКА "; print_attr(atom, 4, os); return;
        case 11: os << "УВЕЛИЧ "; print_attr(atom, 2, os); return;
        case 12: os << "СЛОЖ "; print_attr(atom, 3, os); return;
        case 13: os << "ВЫЧИТ "; print_attr(atom, 3, os); return;
        case 14: os << "УМНОЖ "; print_attr(atom, 3, os); return;
        case 15: os << "ДЕЛЕН "; print_attr(atom, 3, os); return;
        case 16: os << "ЭКСП "; print_attr(atom, 3, os); return;
        case 17: os << "ПЛЮС "; print_attr(atom, 2, os); return;
        case 18: os << "МИНУС "; print_attr(atom, 2, os); return;
        }*/
        os << atom.klass << ' ';
        print_attr(atom, 4, os);
    }

    void print_syntax_info(std::ostream& os = std::cout) {
        for (int i = 0; i < NTA; ++i) {
            os << i << ". ";
            print_atom(TA[i], os);
            os << '\n';
        }
    }
};

class Generator : Syntax
{
    int numstr;
public:
    void start(const char* nameIn, const char* nameOut)
    {
        run_lexer(nameIn);

        std::ofstream lout("lexer_data.txt");
        print_lexer_info(lout);
        lout.close();

        if (iswrong)
            exit(1);

        run_syntax();

        std::ofstream sout("syntax_data.txt");
        print_syntax_info(sout);
        sout.close();

        if (iswrong)
        {
            std::cout << "В программе есть ошибки. Не является возможным сгенерировать код. Придётся чинить..." << std::endl;
            return;
        }
        else
            std::cout << "Ошибок не нашлось... очень жаль, но вы молодец." << std::endl;
        std::ofstream out(nameOut);
        out << "include\\masm32\\include\\masm32rt.inc";
        out << "\n\n.386\n\n.data?\nhout dd ?\n";
        for (int i = 1; i < 286; i++)
        {
            if (TO[i] == 1)
                out << 'x' << i << " dd ?\n";
        }
        for (int i = 700; i < NOVT - NTO; i++)
        {
            out << 'x' << i << " dd ?\n";
        }
        out << " \n.data\n";
        //out<<"x0 dd "<<1<<"\n";
        for (int i = 286; i < NOVT; i++)
        {
            out << 'x' << i << " dd "<<TO[i]<<"\n";
        }
        out << "\n.code\nstart:\n";
        for (int i = 0; i < NTA; i++)
        {
            switch (TA[i].klass)
            {
            case 2:
                out << "label" << TA[i].attributes[0] << ":\n";
                numstr = TA[i].attributes[0];
                break;
            case 3:
                out << "mov eax, x" << TA[i].attributes[1] << "\n";
                out << "mov " <<"x"<< TA[i].attributes[0] << ", eax\n";
                break;
            case 4:
                out << "jmp label" << TA[i].attributes[0] << "\n";
                break;
            case 5:
                out << "push returnlabel" << numstr << "\n";
                out << "jmp label" << TA[i].attributes[0] << "\n";
                out << "returnlabel" << numstr << ":\n";
                break;
            case 6:
                out << "ret\n";
                break;
            case 7:
                out << "mov eax, x" << TA[i].attributes[0] << "\n";
                out << "cmp eax, x"  << TA[i].attributes[1] << "\n";
                switch (TA[i].attributes[2])
                {
                case 1:
                    out << "JE "; break;
                case 2:
                    out << "JL "; break;
                case 3:
                    out << "JG "; break;
                case 4:
                    out << "JLE "; break;
                case 5:
                    out << "JGE "; break;
                case 6:
                    out << "JNE "; break;
                }
                out << "label" << TA[i].attributes[3] << "\n";
                break;
            case 8:
                out << "mov eax, " <<"x" << TA[i].attributes[0] << "\n";
                out << "mov x" << TA[i].attributes[1] << ", eax\n";
                break;
            case 9:
                out << "label" << TA[i].attributes[0] << ":\n";
                break;
            case 10:
                out << "mov eax, x" << TA[i].attributes[2] << "\n";
                out << "cmp eax, 0\n"; 
                out << "JGE label" << TA[i].attributes[3] << "\n";
                out << "mov eax, x" << TA[i].attributes[0] << "\n";
                out << "cmp eax, x"<<TA[i].attributes[1] <<"\n";
                out << "JL label" << TA[i].attributes[3] << "\n";
                out << "jmp labelT" << TA[i].attributes[3] << "\n";
                out << "labelL" << TA[i].attributes[3] << ":\n";
                out << "mov eax, x" << TA[i].attributes[0] << "\n";
                out << "cmp eax, x" << TA[i].attributes[1] << "\n";
                out << "JG label" << TA[i].attributes[3] << "\n";
                out << "labelT" << TA[i].attributes[3] << ":\n";
                break;
            case 11:
                out << "mov eax, x" << TA[i].attributes[0] << "\n";
                out << "add eax, x" << TA[i].attributes[1] << "\n";
                out << "mov x" << TA[i].attributes[0] << ", eax\n";
                break;
            case 12:
                out << "mov eax, x" << TA[i].attributes[0] << "\n";
                out << "add eax, x" << TA[i].attributes[1] << "\n";
                out << "mov x" << TA[i].attributes[2] << ", eax\n";
                break;
            case 13:
                out << "mov eax, x" << TA[i].attributes[0] << "\n";
                out << "sub eax, x" << TA[i].attributes[1] << "\n";
                out << "mov x" << TA[i].attributes[2] << ", eax\n";
                break;
            case 14:
                out << "mov eax, x" << TA[i].attributes[0] << "\n";
                out << "imul eax, x" << TA[i].attributes[1] << "\n";
                out << "mov x" << TA[i].attributes[2] << ", eax\n";
                break;
            case 15:
                out << "mov eax, x" << TA[i].attributes[0] << "\n";
                out << "cdq\n";
                out << "mov ebx, x" << TA[i].attributes[1] << "\n";
                out << "idiv ebx\n";
                out << "mov x" << TA[i].attributes[2] << ", eax\n";
                break;
            case 16:
                out << "mov eax, 1\n";
                out << "mov ecx, x" << TA[i].attributes[1] << "\n";
                out << "L" << numstr << ":\n";
                out << "imul x" << TA[i].attributes[0] << "\n";
                out << "loop L" << numstr << "\n";
                out << "mov x" << TA[i].attributes[2] << ", eax\n";
                break;
            case 17:
                out << "mov eax, x"  << TA[i].attributes[0] << "\n";
                out << "mov x" << TA[i].attributes[1] << ", eax\n";
                break;
            case 18:
                out << "mov eax, x" << TA[i].attributes[0] << "\n";
                out << "imul eax, -1\n";
                out << "mov x" << TA[i].attributes[1] << ", eax\n";
                break;
            }
        }
        out << "\ninvoke GetStdHandle, STD_OUTPUT_HANDLE\n";
        out << "mov hout, eax\n";
        out << "mov eax, x44\n";
        out << "cmp eax, 1\n";
        out << "JE labelR\n";
        out << "print \" NET\"\n";
        out << "jmp labelE\n";
        out << "labelR:\n";
        out << "print \" DA\"\n";
        out << "labelE:\n";
        out << "mov eax, x70\n";
        out << "cmp eax, 1\n";
        out << "JE labelR2\n";
        out << "print \" NET\"\n";
        out << "jmp labelE2\n";
        out << "labelR2:\n";
        out << "print \" DA\"\n";
        out << "labelE2:\n";

        out << "\ninkey\nexit\nend start";
        out.close();
    }
    
};

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Generator L;

    L.start("test.txt", "program.asm");

    return 0;
}