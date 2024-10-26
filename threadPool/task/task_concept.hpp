#ifndef TASK_CONCEPT_HPP
#define TASK_CONCEPT_HPP

#include <concepts>
#include <future>


namespace BaseTool::ThreadPool::Task
{
    template<typename T>
    concept TaskHandler = requires(T t)
    {
        { t.Start() } -> std::convertible_to<bool>;
        { t.Pause() } -> std::convertible_to<bool>;
        { t.Resume() } -> std::convertible_to<bool>;
        { t.Stop() } -> std::convertible_to<bool>;
    };

    template<typename T>
    concept ThreadPoolTask = requires(T t)
    {
        { t.IsRunnable() } -> std::convertible_to<bool>;
    } && std::is_constructible_v<T, std::function<void(void)>, int> && std::invocable<T> && std::totally_ordered<T> && TaskHandler<T>;


    template<typename T,typename Task>
    concept TaskController = TaskHandler<T>;


}

#endif //TASK_CONCEPT_HPP
