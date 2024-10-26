#ifndef TASK_HANDLER_WEAK_HPP
#define TASK_HANDLER_WEAK_HPP

#include "task_concept.hpp"

namespace BaseTool::ThreadPool::Task
{
    template<TaskHandler Task>
    class TaskControllerImpl
    {
    public:
        TaskControllerImpl(const std::weak_ptr<Task> &, const std::weak_ptr<std::condition_variable> &);

        ~TaskControllerImpl() = default;

        TaskControllerImpl(const TaskControllerImpl &) = delete;

        TaskControllerImpl(TaskControllerImpl &&) = delete;

        TaskControllerImpl &operator=(const TaskControllerImpl &) = delete;

        TaskControllerImpl &operator=(TaskControllerImpl &&) = delete;

        bool Start();

        bool Pause();

        bool Resume();

        bool Stop();

    private:
        std::weak_ptr<std::condition_variable> cond_;

        std::weak_ptr<Task> task_;
    };

    template<TaskHandler Task>
    TaskControllerImpl<Task>::TaskControllerImpl(const std::weak_ptr<Task> &task,
                                                 const std::weak_ptr<std::condition_variable> &cond):

        cond_(cond), task_(task)
    {
    }

    template<TaskHandler Task>
    bool TaskControllerImpl<Task>::Pause()
    {
        if (auto sp = task_.lock())
        {
            return sp->Pause();
        }
        return false;
    };

    template<TaskHandler Task>
    bool TaskControllerImpl<Task>::Resume()
    {
        if (auto sp = cond_.lock())
        {
            if (auto spw = task_.lock())
            {
                sp->notify_all();
                return spw->Resume();
            }
        }
        return false;
    }

    template<TaskHandler Task>
    bool TaskControllerImpl<Task>::Start()
    {
        if (auto sp = cond_.lock())
        {
            if (auto spw = task_.lock())
            {
                if (spw->Start())
                {
                    sp->notify_all();
                    return true;
                }
            }
        }

        return false;
    }

    template<TaskHandler Task>
    bool TaskControllerImpl<Task>::Stop()
    {
        if(auto sp = task_.lock())
        {
            return task_->Stop();
        }
        return false;
    }

}

#endif //TASK_HANDLER_WEAK_HPP
