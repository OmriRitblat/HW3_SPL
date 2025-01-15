#include <queue>
#include <mutex>
#include <condition_variable>
#include "event.h"
#include "client/include/ThreadSafeQueue.h"


    void ThreadSafeQueue::enqueue(std::string item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(item));
        condition_.notify_one();
    }

    std::string ThreadSafeQueue::dequeue() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this]() { return !queue_.empty(); }); // Wait until queue is not empty
        std::string item = std::move(queue_.front());
        queue_.pop();
        return item;
    }

    bool ThreadSafeQueue::empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t ThreadSafeQueue::size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }