#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <queue>
#include <concepts>
#include <mutex>
#include <shared_mutex>

namespace BaseTool::ThreadPool::Container
{
    template<typename T,typename Container,typename Com>
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
            std::unique_lock lock(mtx_);
            queue_.emplace(std::forward<Args>(args)...);
        }

        T Top()
        {
            std::unique_lock lock(mtx_);
            return queue_.top();
        }

        void Clear()
        {
            std::unique_lock lock(mtx_);
            while (!queue_.empty())
            {
                queue_.pop();
            }
        }

        void ReSort()
        {
            std::unique_lock lock(mtx_);
            int size = queue_.size();
            for(int i = 0 ; i < size ; i++)
            {
                T val = queue_.top();
                queue_.pop();
                queue_.push(val);
            }
        }

        void Pop()
        {
            std::unique_lock lock(mtx_);
            queue_.pop();
        }

        bool Empty()
        {
            std::shared_lock lock(mtx_);
            return queue_.empty();
        }

        size_t Size()
        {
            std::shared_lock lock(mtx_);
            return queue_.size();
        }

    private:
        std::shared_mutex mtx_;
        std::priority_queue<T,Container,Com> queue_;
    };

}

#endif //PRIORITY_QUEUE_HPP
