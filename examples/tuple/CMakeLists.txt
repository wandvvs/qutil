cmake_minimum_required(VERSION 3.18)
project(tuple-example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(qutil REQUIRED)

add_executable(tuple-example tuple.cpp)

target_link_libraries(tuple-example PRIVATE qutil::qutil)

