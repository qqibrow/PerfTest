#ifndef QQIBROW_LOCKINGBLOCKINGQUEUE
#define QQIBROW_LOCKINGBLOCKINGQUEUE

#include <mutex>
#include <condition_variable>
#include <queue>

#include "BlockingQueue.h"

template <typename T>
class LockBlockingQueue : public BlockingQueue<T> {
public:
    LockBlockingQueue() : notEmpty_(false) {}
    virtual T take() {
        // First make sure queue is not empty.
        std::unique_lock<std::mutex> lock(condition_mutex_);
        while(!notEmpty_) {
            condition_.wait(lock);
        }

        T curr = queue_.front();
        queue_.pop();

        lock.unlock();
        condition_.notify_one();
        return curr;
    }

    virtual void put(T value) {

        {
            std::lock_guard<std::mutex> lk(condition_mutex_);
            queue_.push(value);
            notEmpty_ = true;
        }
        condition_.notify_all();
    }

private:
    std::queue<T> queue_;
    std::mutex condition_mutex_;
    std::condition_variable condition_;
    bool notEmpty_;
};

#endif