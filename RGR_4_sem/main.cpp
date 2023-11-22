

#include <iostream>
#include <fstream>
#include "lexer.h"
#include "syntax.cpp"

int main()
{
    std::ifstream fileStream("input1.txt");
    if (!fileStream.is_open())
        return 1;

    std::string text = std::string(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
    fileStream.close();
    Lexer lexer(text);
    auto lexems = lexer.Start();
    /*for (const auto& lexem : lexems)
    {
        std::cout << lexem << std::endl;
    }*/

    AnalyzerGPP A("data/grammar.txt");
    std::ofstream B("outputs/table.txt");
    A.print_rel_table(B);
    A.run(lexems);

    return 0;
}