cmake_minimum_required(VERSION 3.20)

project(Foundation)

set(CMAKE_CXX_STANDARD 20)

include(cmake/toolchain.cmake)

# 输出编译器信息以供验证
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

include_directories(${PROJECT_SOURCE_DIR}/tool)

add_subdirectory(${PROJECT_SOURCE_DIR}/param)
add_subdirectory(${PROJECT_SOURCE_DIR}/log)
add_subdirectory(${PROJECT_SOURCE_DIR}/threadPool)
