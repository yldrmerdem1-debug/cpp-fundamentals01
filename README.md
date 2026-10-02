# C++ Fundamentals

Small console programs from my C++ fundamentals coursework: arrays, loops, input validation,
basic statistics and a first class with encapsulated state.

| File | Topic |
|---|---|
| `BellCurveGrading.cpp` | Mean and standard deviation of 10 grades, letter grades on a bell curve |
| `weightedscores.cpp` | 2D array of midterm / project / final scores, weighted totals and pass/fail |
| `grade_frequency.cpp` | Frequency table of 100 grades with input validation |
| `grade_counter.cpp` | Counting grades below and above 50 |
| `Time_class_example.cpp` | `Time` class: constructor, validated setter, 24-hour and 12-hour output |

## Build and run

Each file is a standalone program:

```bash
g++ -std=c++17 -Wall BellCurveGrading.cpp -o bell && ./bell
```
