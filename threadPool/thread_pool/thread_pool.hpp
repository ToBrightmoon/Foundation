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
        enum class ThreadPoolStatus
        {
            Pause,
            Running,
            Stop
        };
    public:
        explicit ThreadPool(uint numThreads = std::thread::hardware_concurrency());

        ~ThreadPool();

        template<Task::TaskController<TaskType> ControllerType, typename ReturnType, typename Func, typename... Args>
        std::shared_ptr<ControllerType> AddTask(int, std::future<ReturnType> &, Func &&, Args &&...);

        template<Task::TaskController<TaskType> ControllerType>
        std::shared_ptr<ControllerType> AddTask(int, const std::function<void(void)> &);

        void Start();

        void Stop();

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

        ThreadPoolStatus status_ = ThreadPoolStatus::Pause;

        std::atomic_int reSortTimes_ = 0;

        const int maxTimes_ = 10000;

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
        for (auto &worker: workers_)
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
    template<Task::TaskController<TaskType> ControllerType, typename ReturnType, typename Func, typename... Args>
    std::shared_ptr<ControllerType> ThreadPool<TaskType>::AddTask(int priority, std::future<ReturnType> &reFeture,
                                                                  Func &&f,
                                                                  Args &&... args)
    {
        auto task = std::make_shared<std::packaged_task<ReturnType(void)> >(
            std::bind(std::forward<Func>(f), std::forward<Args>(args)...));

        std::future<ReturnType> future = task->get_future();

        reFeture = std::move(future);

        auto taskRun = std::make_shared<TaskType>([task]() { (*task)(); }, priority);

        auto taskController = std::make_shared<ControllerType>(taskRun, cond_);

        taskQues_.Push(taskRun);

        cond_->notify_all();

        return taskController;
    }

    template<Task::ThreadPoolTask TaskType>
    void ThreadPool<TaskType>::Start()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        status_ = ThreadPoolStatus::Running;
        for (int i = 0; i < numThreads_; ++i)
        {
            workers_.emplace_back(std::jthread(std::bind(&ThreadPool<TaskType>::process, this)));
        }
    }

    template<Task::ThreadPoolTask TaskType>
    void ThreadPool<TaskType>::Stop()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        status_ = ThreadPoolStatus::Stop;
        cond_->notify_all();
    }

    template<Task::ThreadPoolTask TaskType>
    void ThreadPool<TaskType>::process()
    {
        while (true)
        {
            std::shared_ptr<TaskType> task = nullptr;
            {
                std::unique_lock<std::mutex> lock(mtx_);
                cond_->wait(lock, [this]()
                {
                    return (!taskQues_.Empty() && status_ == ThreadPoolStatus::Running)|| ThreadPoolStatus::Stop == status_;
                });

                if(ThreadPoolStatus::Stop == status_)
                {
                    taskQues_.Clear();
                    cond_->notify_all();
                    return;
                }
                task = taskQues_.Top();
                taskQues_.Pop();
            }

            if (task != nullptr)
            {
                if (task->IsRunnable())
                {
                    reSortTimes_.store(0);
                    (*task)();
                }
                else
                {
                    reSortTimes_.store(reSortTimes_.load() + 1);
                    taskQues_.Push(task);
                    taskQues_.ReSort();
                    if(reSortTimes_.load() >= maxTimes_)
                    {
                        std::unique_lock<std::mutex> lock(mtx_);
                        status_ = ThreadPoolStatus::Pause;
                    }
                }
            }
        }
    }
}

#endif //THREAD_POOL_HPP
