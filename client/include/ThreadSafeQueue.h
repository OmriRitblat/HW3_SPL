#include <queue>
#include <mutex>
#include <condition_variable>
#include "event.h"

class ThreadSafeQueue {
private:
    std::queue<std::string> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;

public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    void enqueue(Event item) ;

    std::string dequeue();

    bool empty() const ;
    size_t size() const ;
};