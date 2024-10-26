#include <iostream>

#include "priority_queue.hpp"

int main()
{
    using namespace BaseTool::ThreadPool;

    Container::PriorityQueue<int> pq;

    std::cout << pq.Empty() << "\n";

    std::cout << pq.Size() << "\n";
    pq.Push(10);
    pq.Push(20);
    pq.Push(1);

    auto size = pq.Size();
    for(int i = 0; i < size; i++)
    {
        std::cout << pq.Top() << " ";
        pq.Pop();
    }
    std::cout << "Hello Thread Pool" << "\n";
}
