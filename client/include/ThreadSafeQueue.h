#include <queue>
#include <mutex>
#include <condition_variable>
#include "event.h"

class ThreadSafeQueue {
private:
    std::queue<Event> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;

public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    void enqueue(Event item) ;

    Event dequeue();

    bool empty() const ;
    size_t size() const ;
};