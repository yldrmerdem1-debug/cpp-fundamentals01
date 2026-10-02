#pragma once
#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// The input is not a valid expression; position() is the 0-based character index of the problem.
class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message, std::size_t position)
        : std::runtime_error(message + " at position " + std::to_string(position)), position_(position) {}
    std::size_t position() const { return position_; }

private:
    std::size_t position_;
};

// The expression is valid but has no value (division by zero, unknown variable, ...).
class EvalError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

struct Token {
    enum class Kind { Number, Variable, Operator, LeftParen, RightParen };
    Kind kind;
    std::string text;  // operators: "+", "-", "*", "/", "^" and "neg" for unary minus
    double value = 0.0;
    std::size_t position = 0;
};

// Splits an expression into tokens. A '-' where a value is expected becomes the unary "neg".
std::vector<Token> tokenize(const std::string& expression);

// Shunting-yard: infix tokens to reverse Polish notation, checking the structure on the way.
std::vector<Token> toRpn(const std::vector<Token>& tokens);

// Evaluates expressions such as "rate * (1 + rate) ^ n" with + - * / ^, unary minus,
// parentheses and named variables. Precedence: ^ (right-assoc) > unary minus > * / > + -.
double evaluate(const std::string& expression, const std::map<std::string, double>& variables = {});
