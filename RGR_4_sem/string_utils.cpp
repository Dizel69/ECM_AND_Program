#include <iostream>
#include <string>
#include <vector>

/* возвращает вектор строк, полученный после разделения входной строки по подстроке */
std::vector<std::string> split(std::string input_string, const std::string& delim) {
    std::vector<std::string> splitted_string;
    size_t pos;
    while ((pos = input_string.find(delim)) != std::string::npos) {
        splitted_string.push_back(input_string.substr(0, pos));
        input_string.erase(0, pos + delim.length());
    }
    splitted_string.push_back(input_string);

    return splitted_string;
}

#define PRINT_VECTOR(vec) for (const auto& el : vec) { std::cout << el << ' '; }

/*int main() {
    std::string str = "<listV> -> , V <listV> | eps";
    for (auto& s : split(str, " -> ")) {
        std::cout << s << std::endl;
    }

    return 0;
}*/