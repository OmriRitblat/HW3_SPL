#include <queue>
#include <mutex>
#include <condition_variable>
#include "event.h"
#include "client/include/ThreadSafeQueue.h"


class ThreadSafeQueue {
private:
    std::queue<std::string> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;

public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    void enqueue(std::string item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(item));
        condition_.notify_one();
    }

    std::string dequeue() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this]() { return !queue_.empty(); }); // Wait until queue is not empty
        std::string item = std::move(queue_.front());
        queue_.pop();
        return item;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }
};