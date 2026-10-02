#pragma once
// Minimal test helpers: CHECK records a failure and keeps going, report() sets the exit code for CTest.
#include <cstdlib>
#include <iostream>

namespace check {

inline int& failures() {
    static int count = 0;
    return count;
}

inline int report(const char* suite) {
    if (failures() == 0) {
        std::cout << suite << ": all checks passed\n";
        return EXIT_SUCCESS;
    }
    std::cerr << suite << ": " << failures() << " check(s) failed\n";
    return EXIT_FAILURE;
}

}  // namespace check

#define CHECK(expr)                                                                  \
    do {                                                                             \
        if (!(expr)) {                                                               \
            ++check::failures();                                                     \
            std::cerr << __FILE__ << ":" << __LINE__ << ": CHECK(" #expr ") failed\n"; \
        }                                                                            \
    } while (0)

#define CHECK_THROWS(expr, ExceptionType)                                                         \
    do {                                                                                          \
        bool thrown_ = false;                                                                     \
        try {                                                                                     \
            (void)(expr);                                                                         \
        } catch (const ExceptionType&) {                                                          \
            thrown_ = true;                                                                       \
        }                                                                                         \
        if (!thrown_) {                                                                           \
            ++check::failures();                                                                  \
            std::cerr << __FILE__ << ":" << __LINE__ << ": expected " #ExceptionType " from " #expr "\n"; \
        }                                                                                         \
    } while (0)
