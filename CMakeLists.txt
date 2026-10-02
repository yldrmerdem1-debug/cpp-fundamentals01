cmake_minimum_required(VERSION 3.16)
project(cpp_fundamentals LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Threads REQUIRED)
enable_testing()

add_library(bank bank/account.cpp bank/bank.cpp)
target_include_directories(bank PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(expr expr/evaluator.cpp)
target_include_directories(expr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(bank_demo bank/main.cpp)
target_link_libraries(bank_demo PRIVATE bank)

add_executable(calc expr/main.cpp)
target_link_libraries(calc PRIVATE expr)

add_executable(primes thread_pool/main.cpp)
target_include_directories(primes PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(primes PRIVATE Threads::Threads)

# Fundamentals: small standalone console programs that read from standard input.
foreach(program BellCurveGrading Time_class_example grade_counter grade_frequency weightedscores)
  add_executable(${program} basics/${program}.cpp)
endforeach()

function(add_check name)
  add_executable(${name} tests/${name}.cpp)
  target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tests)
  if(ARGN)
    target_link_libraries(${name} PRIVATE ${ARGN})
  endif()
  add_test(NAME ${name} COMMAND ${name})
endfunction()

add_check(test_lru_cache)
add_check(test_thread_pool Threads::Threads)
add_check(test_bank bank)
add_check(test_expr expr)
