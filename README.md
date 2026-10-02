# C++ Fundamentals and Projects

![CI](https://github.com/yldrmerdem1-debug/cpp-fundamentals01/actions/workflows/ci.yml/badge.svg)

From first console programs to tested C++17 projects covering data structures, concurrency,
object-oriented design and parsing.

## Projects

| Project | What it covers |
|---|---|
| [`lru_cache`](lru_cache/lru_cache.hpp) | Generic least-recently-used cache with O(1) `get`/`put`: class templates, `std::list::splice` plus an `unordered_map` of list iterators, `std::optional`, hit/miss statistics |
| [`thread_pool`](thread_pool/thread_pool.hpp) | Fixed-size thread pool: `std::condition_variable`, `std::packaged_task` and futures, perfect forwarding, exceptions delivered through the future, graceful shutdown that drains the queue |
| [`bank`](bank/) | Account hierarchy: abstract base class, virtual dispatch, custom exception type, money in integer cents, overdraft limits, all-or-nothing transfers, month-end interest and fees |
| [`expr`](expr/) | Expression calculator: tokenizer, shunting-yard conversion to reverse Polish notation, precedence and associativity, unary minus, variables, error messages with the exact position |

Each project has tests in [`tests/`](tests/), run on Linux and Windows by GitHub Actions.

## Fundamentals

Small console programs in [`basics/`](basics/):

| Program | Topic |
|---|---|
| `BellCurveGrading` | Mean and standard deviation of 10 grades, letter grades on a bell curve |
| `weightedscores` | 2D array of midterm / project / final scores, weighted totals and pass/fail |
| `grade_frequency` | Frequency table of 100 grades with input validation |
| `grade_counter` | Counting grades below and above 50 |
| `Time_class_example` | `Time` class: constructor, validated setter, 24-hour and 12-hour output |

## Build and test

Requires CMake 3.16+ and a C++17 compiler (GCC, Clang or MSVC).

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Try it

```bash
./build/calc
> rate = 0.05
rate = 0.05
> 1000 * (1 + rate) ^ 10
1628.89
> 2 * (3 +
error: expression ends without a value at position 7
```

```bash
./build/bank_demo   # statements for a savings and a checking account after one month
./build/primes      # counts primes below 2,000,000 on all cores through the thread pool
```

On Windows with Visual Studio the programs are under `build\Debug\`.
