#include <queue>
#include <mutex>
#include <condition_variable>
#include "event.h"
#include "../include/Frame.h"

class ThreadSafeQueue
{
private:
    std::queue<Frame> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;

public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    void enqueue(std::string item);

    std::string dequeue();

    bool empty() const;
    size_t size() const;
};