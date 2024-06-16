#include "Generator.hpp"
using namespace std;

void Generator::Generate(const char* nameIn, const char* nameOut)
{
    if (!Lexer::Start(nameIn))
    {
        cout << "Обнаружены ошибки в построении лексем.\nСинтаксический блок не запускается\n" << endl << endl;
        for (int i = 0; i < errorsList.size(); i++)
        {
            Lexer::PrintError(errorsList[i]);
        }
        return;
    }

    Lexer::PrintLexer();

    if (!Synt::Start())
        return;

    Synt::printATOM();

    cout << "Ошибок не обнаружено. Началась генерация кода\n";

    ofstream out(nameOut);
    out << "include\\masm32\\include\\masm32rt.inc";
    out << "\n\n.386\n\n.data?\nhout dd ?\n";

    for (int i = 0; i <= 286; i++)
    {
        if (TO[i] == 1)
            out << 'x' << i << " dd ?\n";
    }

    for (int i = 700; i < NOVT - NTO; i++) //
    {
        out << 'x' << i << " dd ?\n";
    }

    out << " \n.data\n";
    for (int i = 287; i < NOVT; i++) //
    {
        out << 'x' << i << " dd " << TO[i] << "\n";
    }

    out << "\n.code\nstart:\n";
    for (int i = 0; i < TA.size(); i++)
    {
        switch (TA[i].cl)
        {
        case 2:
            out << "label" << TA[i].a1 << ":\n";
            numstr = TA[i].a1;
            break;
        case 3:
            out << "mov eax, x" << TA[i].a2 << "\n";
            out << "mov " << "x" << TA[i].a1 << ", eax\n";
            break;
        case 4:
            out << "jmp label" << TA[i].a1 << "\n";
            break;
        case 5:
            out << "push returnlabel" << numstr << "\n";
            out << "jmp label" << TA[i].a1 << "\n";
            out << "returnlabel" << numstr << ":\n";
            break;
        case 6:
            out << "ret\n";
            break;
        case 7:
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "cmp eax, x" << TA[i].a2 << "\n";
            switch (TA[i].a3)
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
            out << "label" << TA[i].a4 << "\n";
            break;
        case 8:
            out << "mov eax, " << "x" << TA[i].a1 << "\n";
            out << "mov x" << TA[i].a2 << ", eax\n";
            break;
        case 9:
            out << "label" << TA[i].a1 << ":\n";
            break;
        case 10:
            out << "mov eax, x" << TA[i].a3 << "\n";
            out << "cmp eax, 0\n";
            out << "JGE label" << TA[i].a4 << "\n";
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "cmp eax, x" << TA[i].a2 << "\n";
            out << "JL label" << TA[i].a4 << "\n";
            out << "jmp labelT" << TA[i].a4 << "\n";
            out << "labelL" << TA[i].a4 << ":\n";
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "cmp eax, x" << TA[i].a2 << "\n";
            out << "JG label" << TA[i].a4 << "\n";
            out << "labelT" << TA[i].a4 << ":\n";
            break;
        case 11:
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "add eax, x" << TA[i].a2 << "\n";
            out << "mov x" << TA[i].a1 << ", eax\n";
            break;
        case 12:
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "add eax, x" << TA[i].a2 << "\n";
            out << "mov x" << TA[i].a3 << ", eax\n";
            break;
        case 13:
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "sub eax, x" << TA[i].a2 << "\n";
            out << "mov x" << TA[i].a3 << ", eax\n";
            break;
        case 14:
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "imul eax, x" << TA[i].a2 << "\n";
            out << "mov x" << TA[i].a3 << ", eax\n";
            break;
        case 15:
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "cdq\n";
            out << "mov ebx, x" << TA[i].a2 << "\n";
            out << "idiv ebx\n";
            out << "mov x" << TA[i].a3 << ", eax\n";
            break;
        case 16:
            out << "mov eax, 1\n";
            out << "mov ecx, x" << TA[i].a2 << "\n";
            out << "L" << numstr << ":\n";
            out << "imul x" << TA[i].a1 << "\n";
            out << "loop L" << numstr << "\n";
            out << "mov x" << TA[i].a3 << ", eax\n";
            break;
        case 17:
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "mov x" << TA[i].a2 << ", eax\n";
            break;
        case 18:
            out << "mov eax, x" << TA[i].a1 << "\n";
            out << "imul eax, -1\n";
            out << "mov x" << TA[i].a2 << ", eax\n";
            break;
        }
    }
    out << "\ninvoke GetStdHandle, STD_OUTPUT_HANDLE\n";
    out << "mov hout, eax\n";
    out << "mov eax, x44\n";
    out << "cmp eax, 1\n";
    out << "JE labelR\n";
    out << "print \" NET \"\n";
    out << "jmp labelE\n";
    out << "labelR:\n";
    out << "print \" DA \"\n";
    out << "labelE:\n";
    out << "mov eax, x70\n";
    out << "cmp eax, 1\n";
    out << "JE labelR2\n";
    out << "print \" NET \"\n";
    out << "jmp labelE2\n";
    out << "labelR2:\n";
    out << "print \" DA \"\n";
    out << "labelE2:\n";

    out << "\ninkey\nexit\nend start";
    out.close();
}
