#include "task_handler_impl.h"

#include <mutex>

namespace BaseTool::ThreadPool::Task
{
    ThreadPoolTaskImpl::ThreadPoolTaskImpl(const std::function<void()> &task,
                                        const int priority) : task_(task), priority_(priority)

    {
    }

    bool ThreadPoolTaskImpl::IsRunnable() const
    {
        std::shared_lock lock(mtx_);
        return status_ == TaskStatus::Runnable;
    }

    bool ThreadPoolTaskImpl::Pause()
    {
        std::unique_lock lock(mtx_);
        if (status_ == TaskStatus::Stopped)
            return false;
        status_ = TaskStatus::Blocking;
        return true;
    }

    bool ThreadPoolTaskImpl::Start()
    {
        std::unique_lock lock(mtx_);
        if (status_ == TaskStatus::Stopped)
            return false;
        status_ = TaskStatus::Runnable;
        return true;
    }

    bool ThreadPoolTaskImpl::Stop()
    {
        std::unique_lock lock(mtx_);
        status_ = TaskStatus::Stopped;
        return true;
    }


    bool ThreadPoolTaskImpl::Resume()
    {
        std::unique_lock lock(mtx_);
        if (status_ == TaskStatus::Stopped || status_ != TaskStatus::Create)
            return false;
        status_ = TaskStatus::Runnable;
        return true;
    }


    void ThreadPoolTaskImpl::operator()()
    {
        task_();
    }


    bool ThreadPoolTaskImpl::operator <(const ThreadPoolTaskImpl &other) const
    {
        std::shared_lock lock(mtx_);

        return status_ < other.status_ || priority_ < other.priority_;
    }


    bool ThreadPoolTaskImpl::operator >(const ThreadPoolTaskImpl &other) const
    {
        std::shared_lock lock(mtx_);

        return status_ > other.status_ || priority_ > other.priority_;
    }


    bool ThreadPoolTaskImpl::operator <=(const ThreadPoolTaskImpl &other) const
    {
        std::shared_lock lock(mtx_);

        return status_ < other.status_ || priority_ <= other.priority_;
    }


    bool ThreadPoolTaskImpl::operator >=(const ThreadPoolTaskImpl &other) const
    {
        std::shared_lock lock(mtx_);
        return status_ > other.status_ || priority_ >= other.priority_;
    }


    bool ThreadPoolTaskImpl::operator==(const ThreadPoolTaskImpl &other) const
    {
        std::shared_lock lock(mtx_);
        return status_ == other.status_ && priority_ == other.priority_;
    }


    bool ThreadPoolTaskImpl::operator!=(const ThreadPoolTaskImpl &other) const
    {
        std::shared_lock lock(mtx_);
        return status_ != other.status_ || priority_ != other.priority_;
    }
}
