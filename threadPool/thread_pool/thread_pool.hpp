#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <functional>

#include "priority_queue.hpp"
#include "task_concept.hpp"

#include <thread>
#include "task_controller.hpp"


namespace BaseTool::ThreadPool
{
    template<Task::ThreadPoolTask TaskType>
    class ThreadPool
    {
    public:
        // explicit ThreadPool(uint numThreads = std::thread::hardware_concurrency());

        explicit ThreadPool(uint numThreads = 1);

        ~ThreadPool();

        template<Task::TaskController<TaskType> ControllerType, typename... Args>
        std::shared_ptr<ControllerType> AddTask(int, const std::function<void(void)> &f, Args &&... args);

        template<Task::TaskController<TaskType> ControllerType>
        std::shared_ptr<ControllerType> AddTask(int, const std::function<void(void)> &f);

        void Start();

    private:
        struct TaskTypePtrCom
        {
            bool operator()(const std::shared_ptr<TaskType> &t1, const std::shared_ptr<TaskType> &t2) const
            {
                return (*t1) < (*t2);
            }
        };

        void process();

        std::mutex mtx_;

        std::vector<std::jthread> workers_;

        std::shared_ptr<std::condition_variable> cond_ = std::make_shared<std::condition_variable>();

        const uint numThreads_ = std::thread::hardware_concurrency();

        Container::PriorityQueue<std::shared_ptr<TaskType>, std::vector<std::shared_ptr<TaskType> >, TaskTypePtrCom>
        taskQues_;
    };

    template<Task::ThreadPoolTask TaskType>
    ThreadPool<TaskType>::ThreadPool(uint numThreads): numThreads_(numThreads)
    {
    }

    template<Task::ThreadPoolTask TaskType>
    ThreadPool<TaskType>::~ThreadPool()
    {
        for(auto &worker : workers_)
        {
            worker.join();
        }
    }


    template<Task::ThreadPoolTask TaskType>
    template<Task::TaskController<TaskType> ControllerType>
    std::shared_ptr<ControllerType> ThreadPool<TaskType>::AddTask(int priority, const std::function<void()> &f)
    {
        auto task = std::make_shared<std::packaged_task<void(void)> >(f);

        std::future<void> future = task->get_future();

        auto taskRun = std::make_shared<TaskType>([task]() { (*task)(); }, priority);

        auto taskController = std::make_shared<ControllerType>(taskRun, cond_);

        taskQues_.Push(taskRun);

        cond_->notify_all();

        return taskController;
    }

    template<Task::ThreadPoolTask TaskType>
    template<Task::TaskController<TaskType> ControllerType, typename... Args>
    std::shared_ptr<ControllerType> ThreadPool<TaskType>::AddTask(int priority, const std::function<void()> &f,
                                                                  Args &&... args)
    {
        using ReturnType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<ReturnType(void)> >(std::bind(f, std::forward<Args>(args)...));

        std::future<ReturnType> future = task->get_future();

        auto taskRun = std::make_shared<TaskType>([task]() { (*task)(); }, priority);

        auto taskController = std::make_shared<ControllerType>(*taskRun, cond_, future);

        taskQues_.push(taskRun);

        cond_->notify_all();

        return taskController;
    }

    template<Task::ThreadPoolTask TaskType>
    void ThreadPool<TaskType>::Start()
    {
        for (int i = 0; i < numThreads_; ++i)
        {
            workers_.emplace_back(std::jthread(std::bind(&ThreadPool<TaskType>::process, this)));
        }
    }

    template<Task::ThreadPoolTask TaskType>
    void ThreadPool<TaskType>::process()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cond_->wait(lock, [this]()
            {
                return !taskQues_.Empty();
            });

            auto task = taskQues_.Top();
            // std::cout << "查询"<< "\n";
            if (task != nullptr)
            {
                if(task->IsRunnable())
                {
                    (*task)();
                    taskQues_.Pop();
                }
            }
        }
    }
}

#endif //THREAD_POOL_HPP
