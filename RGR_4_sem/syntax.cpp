#include <iomanip>
#include <vector>
#include "lexer.h"
#include "grammar.cpp"

class AnalyzerGPP {
    Grammar grammar;

    std::vector<std::vector<int>> relations;

    int rel_table_size() {
        return grammar.m_nonterminals.size() + grammar.m_terminals.size() + 1;
    }

    void build_relations_table() {
        // equals
        for (const Grammar::production& rule : grammar.m_productions) {
            if (rule.body.size() < 2) continue;

            for (int i = 0; i < rule.body.size() - 1; ++i) {
                relations[rule.body[i]][rule.body[i + 1]] = 1;
            }
        }

        // lesses
        for (const Grammar::production& rule : grammar.m_productions) {
            if (rule.body.size() < 2) continue;

            for (int i = 0; i < rule.body.size() - 1; ++i) {
                if (grammar.is_terminal(rule.body[i + 1])) continue;

                auto&& nach = grammar.NACH(rule.body[i + 1]);
                for (uint32_t nach_s : nach)  {
                    if (relations[rule.body[i]][nach_s] != 0 && relations[rule.body[i]][nach_s] != 2) {
                        std::cout << "Грамматика не ПП: конфликт "
                                  << grammar.get_symbol(rule.body[i]) << " и " << grammar.get_symbol(rule.body[i + 1]) << '\n';
                    }

                    relations[rule.body[i]][nach_s] = 2;
                }
            }
        }

        auto&& nach = grammar.NACH(1);
        for (uint32_t nach_s : nach)  {
            relations[0][nach_s] = 2;
        }

        // biggers
        for (const Grammar::production& rule : grammar.m_productions) {
            if (rule.body.size() < 2) continue;

            for (int i = 0; i < rule.body.size() - 1; ++i) {
                if (grammar.is_terminal(rule.body[i])) continue;

                auto&& kon = grammar.KON(rule.body[i]);
                auto&& first = grammar.FIRST( { rule.body[i + 1] }, 1);
                for (uint32_t kon_s : kon) {
                    for (auto first_s : first) {
                        if (relations[kon_s][first_s[0]] != 0 && relations[kon_s][first_s[0]] != 3) {
                            std::cout << "Грамматика не ПП: конфликт "
                                      << grammar.get_symbol(rule.body[i]) << " и " << grammar.get_symbol(rule.body[i + 1]) << '\n';
                        }

                        relations[kon_s][first_s[0]] = 3;
                    } 
                }
            }
        }

        auto&& kon = grammar.KON(1);
        for (uint32_t kon_s : kon) {
            relations[kon_s][0] = 3;
        }
    }

    uint32_t lexem_to_terminal(const Lexem& lexem) {
        try {
            switch (lexem.type) {
            case LexemType::Identifier: return grammar.m_alphabet_map.at("V");
            case LexemType::Const: return grammar.m_alphabet_map.at("C");
            case LexemType::Semicolon: return grammar.m_alphabet_map.at(";");
           // case LexemType::Put: return grammar.m_alphabet_map.at("\"put\"");
            case LexemType::Relation: return grammar.m_alphabet_map.at("rel");
            case LexemType::Comment: return grammar.m_alphabet_map.at("comment");
            case LexemType::Label: return grammar.m_alphabet_map.at("L");
            default: return grammar.m_alphabet_map.at(lexem.name);
            }
        } catch (...) {
            std::cout << lexem << '\n';
            exit(1);
        }
        
    }

    std::vector<uint32_t> create_input(const std::vector<Lexem>& lexems) {
        std::vector<uint32_t> vec;
        for (auto lex : lexems) {
            vec.push_back(lexem_to_terminal(lex));
            //std::cout << lex << "\n";
        }

        return vec;
    }

public:
    AnalyzerGPP(const std::string& path) {
        grammar.load(path);
        //grammar.print_info();
        relations = std::vector<std::vector<int>>(rel_table_size(), std::vector<int>(rel_table_size(), 0));

        //grammar.test_nach();
        //grammar.test_kon();
        build_relations_table();
    }

    void load_grammar(const std::string& path) {
        grammar.load(path);
    }
    void print_grammar() {
        grammar.print_info();
    }

    uint32_string get_substr(const std::vector<uint32_t>& vec, int i) {
        if (i < 0 || i >= vec.size()) return {};

        uint32_string res;
        for (int j = i; j < vec.size(); ++j) {
            res.push_back(vec[j]);
        }

        return res;
    }

    void run(const std::vector<uint32_t>& input1) {
        std::vector<uint32_t> Stack(1, 0);

        auto input = const_cast<std::vector<uint32_t>&> (input1);

        input.push_back(0);

        int i = 0;
        while (true) {
            uint32_t rel = relations[Stack.back()][input[i]];

            if (rel == 0) {
                std::cout << "Ошибка в входном файле\n";
                break;
            }
            else if (rel == 1 || rel == 2) {
                Stack.push_back(input[i]);
                i++;
            }
            else {
                int i2 = Stack.size() - 1;
                int i1 = i2 - 1;
                while (relations[Stack[i1]][Stack[i2]] != 2) {
                    i1--; i2--;
                    if (i1 < 0) {
                        std::cout << "Ошибка при разборе\n";
                        break;
                    }
                }
                if (i1 < 0) {
                    break;
                }

                uint32_string rhs = get_substr(Stack, i2);

                uint32_t head = 0;
                for (const auto& rule : grammar.m_productions) {
                     if (rule.body == rhs) {
                        head = rule.head;
                        break;
                     }
                }

                if (head == 0) {
                    std::cout << "Ошибка, одинаковые правила\n";
                    break;
                }

                for (int i = 0; i < rhs.size(); i++) {
                    Stack.pop_back();
                }

                Stack.push_back(head);

            }

            if (Stack == std::vector<uint32_t>{ 0, 1 } && input[i] == 0) {
                std::cout << "Допуск\n";
                break;
            }
        }
        
    }








    void run(const std::vector<Lexem>& lexems) {
        run(create_input(lexems));
    }

    void print_rel_table(std::ostream& os = std::cout) {
        os << std::left;
        os << "           ";
        for (int i = 1; i <= grammar.m_alphabet_map.size(); ++i) {
            os << std::setw(10) << grammar.get_symbol(i) << " ";
        }
        os << "$\n";
        for (int i = 1; i <= grammar.m_alphabet_map.size(); ++i) {
            os << std::setw(10) << grammar.get_symbol(i) << " ";
            for (int j = 1; j <= grammar.m_alphabet_map.size(); ++j) {
                os << std::setw(10);
                switch (relations[i][j]) {
                    case 0: os << ""; break;
                    case 1: os << "="; break;
                    case 2: os << "<"; break;
                    case 3: os << ">"; break;
                }
                os << " ";
            }
            switch (relations[i][0]) {
                    case 0: os << ""; break;
                    case 1: os << "="; break;
                    case 2: os << "<"; break;
                    case 3: os << ">"; break;
            }
            os << "\n";
        }

        os << std::setw(10) << "$" << " ";
        for (int j = 1; j <= grammar.m_alphabet_map.size(); ++j) {
            os << std::setw(10);
            switch (relations[0][j]) {
                case 0: os << ""; break;
                case 1: os << "="; break;
                case 2: os << "<"; break;
                case 3: os << ">"; break;
            }
            os << " ";
        }
        os << "\n";
    }
};

/*int main() {
    AnalyzerGPP A("data/grammar.txt");

    std::ofstream out("outputs/table.txt");
    A.print_rel_table(out);
    out.close();

    //std::vector<uint32_t> input = {2, 2, 2, 2, 4, 4, 4};
    //A.run(input);
}*/