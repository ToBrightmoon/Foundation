project(thread_pool)

include_directories(task)
include_directories(container)
include_directories(thread_pool)

add_executable(thread_pool_test test/thread_pool_test.cpp
        task/task_controller.hpp
        task/task_handler_impl.cpp)