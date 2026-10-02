#include "thread_pool/thread_pool.hpp"

#include <atomic>
#include <future>
#include <stdexcept>
#include <vector>

#include "check.hpp"

int main() {
    {
        // Sum 1..1'000'000 in ten chunks.
        ThreadPool pool(4);
        std::vector<std::future<long long>> parts;
        for (long long chunk = 0; chunk < 10; ++chunk) {
            parts.push_back(pool.submit([chunk] {
                long long sum = 0;
                for (long long i = chunk * 100000 + 1; i <= (chunk + 1) * 100000; ++i) sum += i;
                return sum;
            }));
        }
        long long total = 0;
        for (auto& part : parts) total += part.get();
        CHECK(total == 500000500000LL);
    }
    {
        // Exceptions travel through the future; arguments are forwarded.
        ThreadPool pool(2);
        auto failing = pool.submit([]() -> int { throw std::runtime_error("boom"); });
        CHECK_THROWS(failing.get(), std::runtime_error);
        auto sum = pool.submit([](int a, int b) { return a + b; }, 2, 3);
        CHECK(sum.get() == 5);
    }
    {
        // The destructor finishes queued work before joining.
        std::atomic<int> done{0};
        {
            ThreadPool pool(3);
            for (int i = 0; i < 100; ++i) pool.submit([&done] { ++done; });
        }
        CHECK(done == 100);
    }
    {
        ThreadPool pool(1);
        pool.shutdown();
        CHECK_THROWS(pool.submit([] { return 1; }), std::runtime_error);
    }
    return check::report("thread_pool");
}
