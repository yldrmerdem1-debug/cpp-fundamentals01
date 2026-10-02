// Interactive calculator. Assign with "x = 3", then use it: "2 * x ^ 2 - 1".
// An empty line or end of input quits.
#include <iostream>
#include <map>
#include <string>

#include "expr/evaluator.hpp"

namespace {

std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    return s.substr(first, s.find_last_not_of(" \t") - first + 1);
}

}  // namespace

int main() {
    std::map<std::string, double> variables;
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line) && !trim(line).empty()) {
        try {
            const auto eq = line.find('=');
            if (eq == std::string::npos) {
                std::cout << evaluate(line, variables) << '\n';
                continue;
            }
            const std::string name = trim(line.substr(0, eq));
            const auto nameTokens = tokenize(name);
            if (nameTokens.size() != 1 || nameTokens[0].kind != Token::Kind::Variable) {
                throw ParseError("left side of '=' must be a variable name", 0);
            }
            const double value = evaluate(line.substr(eq + 1), variables);
            variables[name] = value;
            std::cout << name << " = " << value << '\n';
        } catch (const std::exception& e) {
            std::cout << "error: " << e.what() << '\n';
        }
    }
}
