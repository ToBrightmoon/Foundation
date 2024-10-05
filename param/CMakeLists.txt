project(param)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/json)

add_executable(param_test test/param_test.cpp)