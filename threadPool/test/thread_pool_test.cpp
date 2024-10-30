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

    auto func2 = [](int i)
    {
        std::cout << "The func2 param is: " << i << std::endl;
        return 1;
    };

    auto ctr = pool.AddTask<Task::TaskControllerImpl<Task::ThreadPoolTaskImpl>>(1,func);

    std::future<int> res;
    auto ctr2 = pool.AddTask<Task::TaskControllerImpl<Task::ThreadPoolTaskImpl>>(1,res,func2,1);

    ctr->Start();

    ctr2->Start();

    pool.Start();


    std::cout << "Hello Thread Pool" << "\n";

    try
    {
        std::cout << "The result is: " << res.get() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }


    pool.Stop();
    // std::this_thread::sleep_for(std::chrono::seconds(10));
}
