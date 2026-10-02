#include "expr/evaluator.hpp"

#include <cctype>
#include <cmath>

namespace {

struct OperatorInfo {
    int precedence;
    bool rightAssociative;
};

OperatorInfo info(const std::string& op) {
    if (op == "+" || op == "-") return {1, false};
    if (op == "*" || op == "/") return {2, false};
    if (op == "neg") return {3, true};
    return {4, true};  // "^"
}

bool isDigit(char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; }
bool isNameStart(char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool isNameChar(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }

}  // namespace

std::vector<Token> tokenize(const std::string& s) {
    std::vector<Token> out;
    // A value may start here: at the beginning, after an operator or after '('.
    auto expectValue = [&out] {
        return out.empty() || out.back().kind == Token::Kind::Operator || out.back().kind == Token::Kind::LeftParen;
    };

    std::size_t i = 0;
    while (i < s.size()) {
        const char c = s[i];
        const std::size_t start = i;
        if (std::isspace(static_cast<unsigned char>(c))) {
            ++i;
        } else if (isDigit(c) || c == '.') {
            while (i < s.size() && (isDigit(s[i]) || s[i] == '.')) ++i;
            const std::string text = s.substr(start, i - start);
            std::size_t used = 0;
            double value = 0.0;
            try {
                value = std::stod(text, &used);
            } catch (const std::exception&) {
                used = 0;
            }
            if (used != text.size()) throw ParseError("malformed number '" + text + "'", start);
            out.push_back({Token::Kind::Number, text, value, start});
        } else if (isNameStart(c)) {
            while (i < s.size() && isNameChar(s[i])) ++i;
            out.push_back({Token::Kind::Variable, s.substr(start, i - start), 0.0, start});
        } else if (c == '(' || c == ')') {
            out.push_back({c == '(' ? Token::Kind::LeftParen : Token::Kind::RightParen, std::string(1, c), 0.0, start});
            ++i;
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            ++i;
            if (expectValue()) {
                if (c == '+') continue;  // unary plus changes nothing
                if (c != '-') throw ParseError(std::string("unexpected '") + c + "'", start);
                out.push_back({Token::Kind::Operator, "neg", 0.0, start});
            } else {
                out.push_back({Token::Kind::Operator, std::string(1, c), 0.0, start});
            }
        } else {
            throw ParseError(std::string("unexpected character '") + c + "'", start);
        }
    }
    return out;
}

std::vector<Token> toRpn(const std::vector<Token>& tokens) {
    std::vector<Token> output;
    std::vector<Token> stack;
    bool expectValue = true;

    for (const Token& t : tokens) {
        switch (t.kind) {
            case Token::Kind::Number:
            case Token::Kind::Variable:
                if (!expectValue) throw ParseError("missing operator before '" + t.text + "'", t.position);
                output.push_back(t);
                expectValue = false;
                break;

            case Token::Kind::LeftParen:
                if (!expectValue) throw ParseError("missing operator before '('", t.position);
                stack.push_back(t);
                break;

            case Token::Kind::RightParen:
                if (expectValue) throw ParseError("expected a value before ')'", t.position);
                while (!stack.empty() && stack.back().kind != Token::Kind::LeftParen) {
                    output.push_back(stack.back());
                    stack.pop_back();
                }
                if (stack.empty()) throw ParseError("unmatched ')'", t.position);
                stack.pop_back();
                break;

            case Token::Kind::Operator: {
                if (t.text == "neg") {  // prefix operator: applies to what follows, pops nothing
                    stack.push_back(t);
                    break;
                }
                if (expectValue) throw ParseError("expected a value before '" + t.text + "'", t.position);
                const OperatorInfo current = info(t.text);
                while (!stack.empty() && stack.back().kind == Token::Kind::Operator) {
                    const OperatorInfo top = info(stack.back().text);
                    const bool popFirst = top.precedence > current.precedence ||
                                          (top.precedence == current.precedence && !current.rightAssociative);
                    if (!popFirst) break;
                    output.push_back(stack.back());
                    stack.pop_back();
                }
                stack.push_back(t);
                expectValue = true;
                break;
            }
        }
    }

    if (expectValue) throw ParseError("expression ends without a value", tokens.empty() ? 0 : tokens.back().position);
    while (!stack.empty()) {
        if (stack.back().kind == Token::Kind::LeftParen) throw ParseError("unmatched '('", stack.back().position);
        output.push_back(stack.back());
        stack.pop_back();
    }
    return output;
}

double evaluate(const std::string& expression, const std::map<std::string, double>& variables) {
    std::vector<double> values;
    for (const Token& t : toRpn(tokenize(expression))) {
        if (t.kind == Token::Kind::Number) {
            values.push_back(t.value);
        } else if (t.kind == Token::Kind::Variable) {
            const auto it = variables.find(t.text);
            if (it == variables.end()) throw EvalError("unknown variable '" + t.text + "'");
            values.push_back(it->second);
        } else if (t.text == "neg") {
            values.back() = -values.back();
        } else {
            // toRpn has already checked that every binary operator has two operands.
            const double rhs = values.back();
            values.pop_back();
            double& lhs = values.back();
            if (t.text == "+") {
                lhs += rhs;
            } else if (t.text == "-") {
                lhs -= rhs;
            } else if (t.text == "*") {
                lhs *= rhs;
            } else if (t.text == "/") {
                if (rhs == 0.0) throw EvalError("division by zero");
                lhs /= rhs;
            } else {
                lhs = std::pow(lhs, rhs);
                if (std::isnan(lhs)) throw EvalError("result is not a real number");
            }
        }
    }
    return values.back();
}
