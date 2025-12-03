#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

namespace cmw {

template <typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(value));
        }
        cv_.notify_one();
    }

    bool try_pop(T& out) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        out = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    T wait_and_pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [&]{ return !queue_.empty(); });
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cv_;
};

} // namespace cmw
