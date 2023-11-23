#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

#include "string_utils.cpp"

/* слова из символов грамматики хранятся в виде числовых строк */
using uint32_string = std::basic_string<uint32_t>;

class Grammar {
private:
    friend class AnalyzerGPP;

    /* множества терминалов и нетерминалов ;
       символы грамматики представлены положительными целыми числами */
    std::set<uint32_t> m_nonterminals; // нетерминалы
    std::set<uint32_t> m_terminals;    // терминалы
    uint32_t m_starting_nonterminal;   // начальный нетерминал

    /* структура "правило грамматики" */
    struct production {
        uint32_t head; // нетерминал
        uint32_string body;  // порождаемое слово

        bool operator ==(const production& other) {
            if (head != other.head) return false;
            if (body != other.body) return false;
            return true;
        }
    };
    /* правила хранятся в виде вектора (т.к. правила необходимо нумеровать) */
    std::vector<production> m_productions;

    /* отображение строк-символов грамматики в числа-символы грамматики */
    std::map<std::string, uint32_t> m_alphabet_map;

    /* вовзвращает первый незанятый номер для нового символа :
       это сумма размеров множеств терминалов и нетерминалов плюс 1 (т.к. нумерация символов начинается с 1) */
    uint32_t next_free_id() {
        return m_nonterminals.size() + m_terminals.size() + 1;
    }

    /* преобразование вектора строк в числовую строку грамматики */
    uint32_string convert_to_istring(const std::vector<std::string>& symbol_seq) {
        /* создание строки из symbol_seq.size() нулевых элементов */
        uint32_string istr(symbol_seq.size(), 0);
        /* заполнение числовой строки зачениями, соответствующими символьным строкам вектора */
        std::generate(istr.begin(), istr.end(),
                      [&, i = 0] () mutable { return m_alphabet_map.at(symbol_seq[i++]); } );

        return istr;
    }

    /* преобразование числовой строки грамматики в вектор соответствующих строк */
    std::vector<std::string> convert_to_vector_string(const uint32_string& istr) {
        /* создание вектора из istr.size() пустых строк */
        std::vector<std::string> vec_str(istr.size(), "");
        /* заполнение вектора строками, соответствующими значениям числовой строки */
        std::generate(vec_str.begin(), vec_str.end(),
                      [&, i = 0] () mutable { return get_symbol(istr[i++]); });

        return vec_str;
    }

    bool is_terminal(uint32_t s) {
        return m_terminals.find(s) != m_terminals.end();
    }

    bool is_nonterminal(uint32_t s) {
        return m_nonterminals.find(s) != m_nonterminals.end();
    }

    bool is_terminal_word(const uint32_string& istr) {
        for (auto s : istr) {
            if (is_nonterminal(s)) return false;
        }
        return true;
    }

    /* стуктура "множества FIRST" */
    struct FIRST_map {
        std::map<uint32_t, std::set<uint32_string>> map; // отображение символов грамматки в множество слов
                                                   // ( соответствующее множество FIRST )
        uint32_t k = 0;                            // параметр FIRST

        void clear() {
            map.clear();
            k = 0;
        }
    };
    /* последняя инициализированная система множеств FIRST и EFF */
    FIRST_map m_FIRST;
    FIRST_map m_EFF;

    /* усечённая конкатенация */
    static std::set<uint32_string> trunccat(const std::set<uint32_string>& _left, const std::set<uint32_string>& _right, uint32_t k) {
        std::set<uint32_string> _truccatted;

        if (_left.empty() || _right.empty()) return _truccatted;
        
        for (const uint32_string& istr_left : _left) {
            if (istr_left.size() >= k) {
                _truccatted.emplace(istr_left.begin(), istr_left.begin() + k);
                continue;
            }
            for (const uint32_string& istr_right : _right) {
                uint32_string _candidate = istr_left + istr_right;
                if (_candidate.size() >= k)
                    _truccatted.emplace(_candidate.begin(), _candidate.begin() + k);
                else
                    _truccatted.insert(std::move(_candidate));
            }
        }

        return _truccatted;
    }

    /* инициализация m_FIRST для параметра k */
    void FIRST_init(uint32_t k) {
        if (m_FIRST.k == k) return;

        /* реализованный алгоритм требует наличия двух отображений "x --- FIRST(x)"
           т.к. после очередной итерации алгоритма необходимо сравнить результат с результатом предыдущего шага
        */
        std::map<uint32_t, std::set<uint32_string>> FIRST_map_1; // первое отображение ( предыдущий шаг )
        std::map<uint32_t, std::set<uint32_string>> FIRST_map_2; // второе отображение ( текущий шаг )

        std::map<uint32_t, std::set<uint32_string>> EFF_map_1; // первое отображение ( предыдущий шаг )
        std::map<uint32_t, std::set<uint32_string>> EFF_map_2; // второе отображение ( текущий шаг )

        /* начальная инициализаци --- базис:
           для каждого нетерминала грамматики составить своё начальное множество слов */
        for (uint32_t nonterminal : m_nonterminals) {
            std::set<uint32_string> set_of_words_F;
            std::set<uint32_string> set_of_words_E;
            /* для каждого правила, в котором текущий нетерминал является левой частью
               составить слово first_k на основе правой части правила такого вида:
                    --- first_k - терминальное слово
                    --- либо first_k.size() == k
                        либо first_k.size() < k, но оно целиком составляет правую часть правила
            */
            for (const production& _production : m_productions) {
                if (_production.head != nonterminal) continue;

                /* если длина правой части правила больше k, то отрезать лишнее */
                uint32_string first_k = _production.body;
                if (first_k.size() >= k)
                    first_k = uint32_string(first_k.begin(), first_k.begin() + k);

                /* если в результате получилось терминальное слово, то добавить его в множество */
                if (is_terminal_word(first_k)){
                    set_of_words_F.insert(first_k);

                    if (!first_k.empty())
                        set_of_words_E.insert(first_k);
                }
                
            }

            /* добавить пару "текущий нетерминал --- множество слов" в формируемое отображение */
            FIRST_map_2.emplace(nonterminal, set_of_words_F);
            EFF_map_2.emplace(nonterminal, set_of_words_E);
        }
        /* для каждого терминала x : FIRST(x) = { x }
           для каждого терминала добваить соответствующую пару в отображение */
        for (uint32_t terminal : m_terminals) {
            FIRST_map_2.emplace(terminal, std::set<uint32_string>{ { terminal } });
            EFF_map_2.emplace(terminal, std::set<uint32_string>{ { terminal } });
        }  

        /* главный цикл алгоритма */
        do {
            /* копирование результатов предыдущего шага */
            FIRST_map_1 = FIRST_map_2;
            EFF_map_1 = EFF_map_2;

            /* для каждой пары в отображении необходимо сформировать некоторое дополнение множества слов
               на основе правил, где соответствующий нетерминал в левой части */
            for (auto it_F = FIRST_map_2.begin(), it_E = EFF_map_2.begin();
                 it_F != FIRST_map_2.end() || it_E != EFF_map_2.end();
                 ++it_F, ++it_E)
            {
                uint32_t nonterminal = (*it_F).first;
                std::set<uint32_string>& set_of_words_F = (*it_F).second;
                std::set<uint32_string>& set_of_words_E = (*it_E).second;

                /* если рассматривается множество, соответствующее терминалу, то пропустить */
                if (is_terminal_word( { nonterminal } )) continue;

                /* для каждого правила, в котором текущий нетерминал является левой частью
                   сформировать дополнение множества слов
                */
                for (const production& _production : m_productions) {
                    if (_production.head != nonterminal) continue;

                    std::set<uint32_string> addition_F; // дополнение для множества слов
                    std::set<uint32_string> addition_E;

                    /* если правая часть правила - пустое слово, то дополнение будет содержать только пустое слово */
                    if (_production.body.empty()) {
                        addition_F = { { } };
                    }
                    /* иначе дополнение будет представлять собой усеченную конкатенацию
                       FIRST_map_1(X) для каждого X из правой части правила */
                    else {
                        addition_F = FIRST_map_1[_production.body[0]];
                        addition_E = EFF_map_1[_production.body[0]];
                        for (size_t i = 1; i < _production.body.size(); ++i) {
                            /* если на каком то этапе дополнение стало пустым множеством,
                               то этот результат уже никак не изменится 
                               т.к конкатенаци с пустым множеством - пустое множество
                            */
                            if (addition_F.empty() && addition_E.empty()) break;

                            addition_F = trunccat(addition_F, FIRST_map_1[_production.body[i]], k);
                            addition_E = trunccat(addition_E, FIRST_map_1[_production.body[i]], k);
                        }
                    }
                    
                    /* объединение с основным множеством */
                    set_of_words_F.merge(addition_F);
                    set_of_words_E.merge(addition_E);
                }
            }
        /* продолжать работу пока результаты двух соседних шагов отличаются */
        } while (FIRST_map_1 != FIRST_map_2 || EFF_map_1 != EFF_map_2);

        /* инициализация поля грамматики */
        m_FIRST.map = std::move(FIRST_map_1);
        m_FIRST.k = k;

        m_EFF.map = std::move(EFF_map_1);
        m_EFF.k = k;
    }

    std::set<uint32_t> NACH(uint32_t symbol, std::set<uint32_t> cache = {}) {
        if (is_terminal(symbol))
            return { symbol };

        std::set<uint32_t> res;
        for (const Grammar::production& rule : m_productions) {
            if (symbol != rule.head) continue;
            
            if (rule.body.empty()) {
                std::cout << "грамматика не ПП: есть пустые правила\n";
                exit(1);
            }
            res.insert(rule.body[0]);

            if (rule.body[0] == symbol) continue;

            if (is_nonterminal(rule.body[0]))
                if (cache.find(rule.body[0]) == cache.end()) {
                    cache.merge(std::set<uint32_t>{symbol});
                    res.merge( NACH(rule.body[0], cache) );
                }
                    
        }
        return res;
    }

    std::set<uint32_t> KON(uint32_t symbol, std::set<uint32_t> cache = {}) {
        if (is_terminal(symbol))
            return { symbol };

        std::set<uint32_t> res;
        for (const Grammar::production& rule : m_productions) {
            if (symbol != rule.head) continue;
            
            if (rule.body.empty()) {
                std::cout << "грамматика не ПП: есть пустые правила\n";
                exit(1);
            }
            res.insert(rule.body.back());

            if (rule.body.back() == symbol) continue;

            if (is_nonterminal(rule.body.back()))
                if (cache.find(rule.body.back()) == cache.end()) {
                    cache.merge(std::set<uint32_t>{symbol});
                    res.merge( KON(rule.body.back(), cache) );
                }  
        }
        return res;
    }
public:
    void clear() {
        m_nonterminals.clear();
        m_terminals.clear();
        m_starting_nonterminal = 0;
        m_productions.clear();
        m_alphabet_map.clear();
        m_FIRST.clear();
        m_EFF.clear();
    }

    /* процедура добавления нетерминала в грамматику */
    void add_nonterminal(const std::string& nonterminal) {
        /* пустая строка игнорируется */
        if (nonterminal.empty() || nonterminal == "\\e") return;

        /* получение номера данной из отображения (если такой там ещё нет, то она будет добавлена со значением 0) */
        uint32_t& nonterminal_id = m_alphabet_map[nonterminal];
        /* если вернулся 0, то добавить новый номер в множество нетерминалов */
        if (nonterminal_id == 0) {
            nonterminal_id = next_free_id();
            m_nonterminals.insert(nonterminal_id);

            /* если это было первое добавление, то инициализировать начальный нетерминал */
            if (m_nonterminals.size() == 1)
                m_starting_nonterminal = nonterminal_id;
        }
        /* если такая строка уже есть грамматкие, то нужно убрать ей из множества терминлов
           и добавить в множество нетерминалов ( если это и так было выполнено, то ничего не произойдёт )*/
        else {
            m_terminals.erase(nonterminal_id);
            m_nonterminals.insert(nonterminal_id);
        }
    }

    /*void set_starting_nonterminal(const std::string& nonterminal) {
        m_starting_nonterminal = m_alphabet_map.at(nonterminal);
    }*/

    /* процедура добавления терминала */
    void add_terminal(const std::string& terminal) {
        /* пустая строка игнорируется */
        if (terminal.empty() || terminal == "\\e") return;

        /* получение номера данной из отображения (если такой там ещё нет, то она будет добавлена со значением 0) */
        uint32_t& terminal_id = m_alphabet_map[terminal];
         /* если вернулся 0, то добавить новый номер в множество терминалов */
        if (terminal_id == 0) {
            terminal_id = next_free_id();
            m_terminals.insert(terminal_id);
        }
    }

    /* процедура добавления правила */
    void add_rule(const std::string& str_head, std::vector<std::string> str_body) {
        /* eps не может быть нетерминалом */
        if (str_head == "\\e") return;
        
        /* удаление пустых слов из строки */
        for (auto it = str_body.begin(); it != str_body.end();) {
            if (*it == "\\e") {
                it = str_body.erase(it);
                continue;
            }
            ++it;
        }

        /* добавление нетерминала из левой части в грамматику */
        add_nonterminal(str_head);
        
        /* добавление каждой строки из правой части в грамматику в качестве терминала */
        //std::for_each(str_body.begin(), str_body.end(), add_terminal); не получилось, т.к. add_terminal - метод класса
        for (const auto& sym : str_body) {
            add_terminal(sym);
        }

        /* создание правила */
        production new_production;
        /* инициализация левой части */
        new_production.head = m_alphabet_map[str_head];
        /* инициализация правой части */
        new_production.body = convert_to_istring(str_body);

        /* проверка на то, нет ли уже такого правила в грамматике */
        for (const auto& _production : m_productions) {
            if (new_production == _production) return;
        }

        /* добавление правила */
        m_productions.push_back(std::move(new_production));
    }

    /* чтение грамматики из файла */
    void load(const std::string& path) {
        clear();

        std::ifstream in(path);

        if (!in) {
            std::cout << "Не удалось открыть файл " << path << std::endl;
            return;
        }

        char input[128];
        while (in) {
            in.getline(input, 128, '\n');

            /* разделение строки из файла под подстроке " -> "
               production_string[0] --- нетерминал правила 
               production_string[1] --- несколько правых частей правила для данного нетерминала,
                                        разделённых подстрокой  " | "
            */
            std::vector<std::string> production_string = split(input, " -> ");
            if (production_string.size() != 2) continue;

            /* добавление всех соответствующих считанной строке правил
              (отдельные символы грамматики должны быть разделены одним пробелом) */
            for (std::string& production_rhs : split(production_string[1], " | ")) {
                add_rule(production_string[0], split(production_rhs, " "));
            }
        }

        // FIRST_init(2);
    }

    /* возвращает строку, соответствующую данному номеру */
    const std::string get_symbol(uint32_t symbol_id) {
        if (symbol_id == 0) return "\\e";

        for (const auto& [str, id] : m_alphabet_map) {
            if (id == symbol_id) return str;
        }

        throw std::out_of_range("Grammar::get_symbol");
    }

    /* для заданного слова word вовзращает FIRST_k(word) в виде множества строк */
    std::set<uint32_string> FIRST(const uint32_string& word, uint32_t k) {
        if (m_FIRST.k != k) FIRST_init(k);

        if (word.empty()) {
            return { {} };
        }
        
        std::set<uint32_string> _res = m_FIRST.map[word[0]];
        for (size_t i = 1; i < word.size(); ++i) {
            if (_res.empty()) break;
            _res = trunccat(_res, m_FIRST.map[word[i]], k);
        }

        return _res;
    }

    /* для заданного слова word вовзращает EFF_k(word) в виде множества строк */
    std::set<uint32_string> EFF(const uint32_string& word, uint32_t k) {
        if (m_EFF.k != k) FIRST_init(k);

        if (word.empty()) {
            return {};
        }
        
        std::set<uint32_string> _res = m_EFF.map[word[0]];
        for (size_t i = 1; i < word.size(); ++i) {
            if (_res.empty()) break;
            _res = trunccat(_res, m_FIRST.map[word[i]], k);
        }

        return _res;
    }

    /* печать грамматики */
    void print_info() {
        std::cout << "Алфавит:\n";
        for (const auto& [str, id] : m_alphabet_map) {
            std::cout << str << " - " << id << '\n';
        }
        std::cout << std::endl;

        std::cout << "Нетерминалы:\n";
        for (uint32_t nt : m_nonterminals) {
            std::cout << get_symbol(nt) << " - " << nt << '\n';
        }
        std::cout << std::endl;

        std::cout << "Терминалы:\n";
        for (uint32_t t : m_terminals) {
            std::cout << get_symbol(t) << " - " << t << '\n';
        }
        std::cout << std::endl;

        std::cout << "Правила:\n";
        for (size_t i = 0; i < m_productions.size(); ++i) {
            std::cout << i << ". " << get_symbol(m_productions[i].head) << " -> ";
            if (m_productions[i].body.empty()) {
                std::cout << "\\e\n";
                continue;
            }
            for (auto s : m_productions[i].body) {
                std::cout << get_symbol(s) << ' ';
            }
            std::cout << '\n';
        }
        std::cout << std::endl;
    }

    void print_word(const uint32_string& istr, std::ostream& os = std::cout) {
        for (uint32_t s : istr) {
            os << get_symbol(s) << ' ';
        }
    }

    void print_set_of_words(const std::set<uint32_string>& istr_set) {
        for (const auto& istr : istr_set) {
            print_word(istr);
            std::cout << '\n';
        }
    }

    void test_nach() {
        for (uint32_t nt : m_nonterminals) {
            std::cout << "НАЧ( " << get_symbol(nt) << " ) { ";
            for (uint32_t s : NACH(nt)) {
                std::cout << get_symbol(s) << ' ';
            }
            std::cout << "}\n";
        }
    }

    void test_kon() {
        for (uint32_t nt : m_nonterminals) {
            std::cout << "КОН( " << get_symbol(nt) << " ) { ";
            for (uint32_t s : KON(nt)) {
                std::cout << get_symbol(s) << ' ';
            }
            std::cout << "}\n";
        }
    }
};