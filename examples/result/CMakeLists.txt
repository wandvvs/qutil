cmake_minimum_required(VERSION 3.18)
project(result-example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(qutil REQUIRED)

add_executable(result-example result.cpp)

target_link_libraries(result-example PRIVATE qutil::qutil)
