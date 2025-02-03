cmake_minimum_required(VERSION 3.18)
project(read-example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(qutil REQUIRED)

add_executable(read-example read.cpp)

target_link_libraries(read-example PRIVATE qutil::qutil)