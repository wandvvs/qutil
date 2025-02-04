cmake_minimum_required(VERSION 3.18)
project(tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(qutil REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#list(APPEND CMAKE_PREFIX_PATH "~/")
find_package(benchmark REQUIRED)

add_executable(benchmarks main_benchmark.cpp)
target_link_libraries(benchmarks benchmark::benchmark)

#add_subdirectory(benchmark)

enable_testing()

add_executable(
  tests
  constexpr_string_test.cpp
)
target_link_libraries(
  tests
  GTest::gtest_main
  qutil::qutil
)

include(GoogleTest)
gtest_discover_tests(tests)