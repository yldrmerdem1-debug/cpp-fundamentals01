// Counts primes below two million on every core and prints how long it took.
#include <chrono>
#include <future>
#include <iostream>
#include <vector>

#include "thread_pool/thread_pool.hpp"

namespace {

bool isPrime(unsigned n) {
    if (n < 2) return false;
    for (unsigned d = 2; d * d <= n; ++d) {
        if (n % d == 0) return false;
    }
    return true;
}

unsigned countPrimes(unsigned from, unsigned to) {
    unsigned count = 0;
    for (unsigned n = from; n < to; ++n) {
        if (isPrime(n)) ++count;
    }
    return count;
}

}  // namespace

int main() {
    constexpr unsigned limit = 2'000'000;
    constexpr unsigned chunks = 16;

    ThreadPool pool;
    const auto start = std::chrono::steady_clock::now();

    std::vector<std::future<unsigned>> parts;
    for (unsigned i = 0; i < chunks; ++i) {
        parts.push_back(pool.submit(countPrimes, limit / chunks * i, limit / chunks * (i + 1)));
    }
    unsigned total = 0;
    for (auto& part : parts) total += part.get();

    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    std::cout << total << " primes below " << limit << " (" << pool.size() << " threads, " << ms.count() << " ms)\n";
}
