#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <queue>
#include <concepts>
#include <mutex>
#include <shared_mutex>

namespace BaseTool::ThreadPool::Container
{
    template<typename T>
        requires std::totally_ordered<T>
    class PriorityQueue
    {
    public:
        PriorityQueue() = default;
        ~PriorityQueue() = default;

        void Push(T value)
        {
            std::unique_lock lock(mtx_);
            queue_.push(std::move(value));
        }

        template<typename ... Args>
        void Emplace(Args&& ... args)
        {
            std::unique_lock<std::shared_mutex> lock(mtx_);
            queue_.emplace(std::forward<Args>(args)...);
        }

        T Top() const
        {
            std::shared_lock lock(mtx_);
            return queue_.top();
        }

        void Pop()
        {
            std::unique_lock lock(mtx_);
            queue_.pop();
        }

        bool Empty() const
        {
            std::shared_lock lock(mtx_);
            return queue_.empty();
        }

        size_t Size() const
        {
            std::shared_lock lock(mtx_);
            return queue_.size();
        }

    private:
        mutable std::shared_mutex mtx_;
        std::priority_queue<T> queue_;
    };

}

#endif //PRIORITY_QUEUE_HPP
