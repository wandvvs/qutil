cmake_minimum_required(VERSION 3.18)
project(qutil VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(qutil INTERFACE)

target_include_directories(qutil INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(DIRECTORY include/ DESTINATION include)

install(TARGETS qutil
    EXPORT qutil_targets
)

install(EXPORT qutil_targets
    FILE qutil-config.cmake
    NAMESPACE qutil::
    DESTINATION lib/cmake/qutil
)

option(BUILD_EXAMPLES "Build example projects" OFF)

if(BUILD_EXAMPLES)
    add_subdirectory(examples/tuple)
endif()

