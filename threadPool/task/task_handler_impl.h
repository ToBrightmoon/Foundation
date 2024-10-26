#ifndef TASK_HANDLER_IMPL_H
#define TASK_HANDLER_IMPL_H

#include <shared_mutex>
#include <functional>

namespace BaseTool::ThreadPool::Task
{
    class ThreadPoolTaskImpl
    {
        enum class TaskStatus : int32_t
        {
            Create,
            Blocking,
            Stopped,
            Runnable
        };

    public:
        ThreadPoolTaskImpl() = default;

        explicit ThreadPoolTaskImpl(const std::function<void()> &task, int priority = 0);

        bool Start();

        bool Pause();

        bool Resume();

        bool Stop();

        bool IsRunnable() const;

        void operator()();

        bool operator <(const ThreadPoolTaskImpl &other) const;

        bool operator >(const ThreadPoolTaskImpl &other) const;

        bool operator <=(const ThreadPoolTaskImpl &other) const;

        bool operator >=(const ThreadPoolTaskImpl &other) const;

        bool operator ==(const ThreadPoolTaskImpl &other) const;

        bool operator !=(const ThreadPoolTaskImpl &other) const;

    private:
        TaskStatus status_ = TaskStatus::Create;

        std::function<void()> task_;

        mutable std::shared_mutex mtx_;

        const int priority_;
    };
}


#endif //TASK_HANDLER_IMPL_H
