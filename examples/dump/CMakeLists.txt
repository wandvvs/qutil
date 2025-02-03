cmake_minimum_required(VERSION 3.18)
project(dump-example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(qutil REQUIRED)

add_executable(dump-example dump.cpp)

target_link_libraries(dump-example PRIVATE qutil::qutil)