cmake_minimum_required(VERSION 3.18)
project(constexpr_string-example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(qutil REQUIRED)

add_executable(constexpr_string-example constexpr_string.cpp)

target_link_libraries(constexpr_string-example PRIVATE qutil::qutil)
