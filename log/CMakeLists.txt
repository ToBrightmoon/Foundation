project(log)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/appender)
include_directories(${CMAKE_CURRENT_LIST_DIR}/container)
include_directories(${CMAKE_CURRENT_LIST_DIR}/format)

add_executable(base_file_logger_test test/base_file_logger_test.cpp)
add_executable(format_time_logger_test test/format_time_file_test.cpp)