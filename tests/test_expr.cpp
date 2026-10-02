#include <cmath>

#include "check.hpp"
#include "expr/evaluator.hpp"

namespace {
bool near(double a, double b) { return std::fabs(a - b) < 1e-9; }
}  // namespace

int main() {
    CHECK(near(evaluate("1 + 2 * 3"), 7));
    CHECK(near(evaluate("(1 + 2) * 3"), 9));
    CHECK(near(evaluate("8 - 3 - 2"), 3));     // left-associative
    CHECK(near(evaluate("2 ^ 3 ^ 2"), 512));   // right-associative
    CHECK(near(evaluate("-2 ^ 2"), -4));       // unary minus binds looser than ^
    CHECK(near(evaluate("2 * -3"), -6));
    CHECK(near(evaluate("2 ^ -1"), 0.5));
    CHECK(near(evaluate("+4 - -1"), 5));
    CHECK(near(evaluate("10 / 4 - .5"), 2));
    CHECK(near(evaluate("rate * (1 + rate) ^ n", {{"rate", 0.5}, {"n", 2}}), 1.125));

    CHECK_THROWS(evaluate("1 / (2 - 2)"), EvalError);
    CHECK_THROWS(evaluate("x + 1"), EvalError);
    CHECK_THROWS(evaluate("(-8) ^ 0.5"), EvalError);

    CHECK_THROWS(evaluate(""), ParseError);
    CHECK_THROWS(evaluate("2 +"), ParseError);
    CHECK_THROWS(evaluate("(1 + 2"), ParseError);
    CHECK_THROWS(evaluate("1 + 2)"), ParseError);
    CHECK_THROWS(evaluate("2 3"), ParseError);
    CHECK_THROWS(evaluate("2 (3)"), ParseError);
    CHECK_THROWS(evaluate("1.2.3"), ParseError);
    CHECK_THROWS(evaluate("4 $ 2"), ParseError);

    try {
        evaluate("1 + * 2");
        CHECK(false);
    } catch (const ParseError& e) {
        CHECK(e.position() == 4);
    }
    return check::report("expr");
}
