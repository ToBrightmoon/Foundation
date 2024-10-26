#include <iostream>

#include "task_concept.hpp"
#include "thread_pool.hpp"
#include "task_handler_impl.h"

int main()
{
    using namespace BaseTool::ThreadPool;

    ThreadPool<Task::ThreadPoolTaskImpl> pool;

    auto func = []()
    {
        std::cout << "The func Hello World!\n";
        return 1;
    };
    auto ctr = pool.AddTask<Task::TaskControllerImpl<Task::ThreadPoolTaskImpl>>(1,func);
    //
    auto ctr2 = pool.AddTask<Task::TaskControllerImpl<Task::ThreadPoolTaskImpl>>(2,func);


    // ctr->Start();

    ctr2->Start();

    pool.Start();


    std::cout << "Hello Thread Pool" << "\n";

    // std::this_thread::sleep_for(std::chrono::seconds(10));
}
