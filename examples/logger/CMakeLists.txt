cmake_minimum_required(VERSION 3.18)
project(logger-example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(qutil REQUIRED)

add_executable(logger-example logger.cpp)

target_link_libraries(logger-example PRIVATE qutil::qutil)