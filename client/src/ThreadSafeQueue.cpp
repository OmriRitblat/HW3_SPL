#include <queue>
#include <mutex>
#include <condition_variable>
#include "event.h"
#include "../include/ThreadSafeQueue.h"


    void ThreadSafeQueue::enqueue(Frame &frame) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(frame);
        condition_.notify_one();
    }

    Frame ThreadSafeQueue::dequeue(){
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this]() { return !queue_.empty(); }); // Wait until queue is not empty
        Frame front = queue_.front(); // Access the front element as an lvalue
        queue_.pop();
        return front;
    }

    bool ThreadSafeQueue::empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t ThreadSafeQueue::size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }