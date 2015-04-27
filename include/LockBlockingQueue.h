#ifndef QQIBROW_LOCKINGBLOCKINGQUEUE
#define QQIBROW_LOCKINGBLOCKINGQUEUE

#include <mutex>
#include <condition_variable>
#include <queue>

#include "Queue.h"

/*
 * ATTENTION: this file is not used in the test.
 * A very basic blocking queue implemented using std::queue and std::mutex.
 * take() and put() operation share the same global mutex.
 */

template <typename T>
class LockBlockingQueue : public Queue<T> {
public:
    LockBlockingQueue() : notEmpty_(false) {}
    virtual ~LockBlockingQueue() {}

    virtual T take() {
        // First make sure queue is not empty.
        std::unique_lock<std::mutex> lock(condition_mutex_);
        while(!notEmpty_) {
            condition_.wait(lock);
        }

        T curr = queue_.front();
        queue_.pop();

        lock.unlock();
        return curr;
    }

    virtual void put(T value) {
        {
            std::lock_guard<std::mutex> lk(condition_mutex_);
            queue_.push(value);
            notEmpty_ = true;
        }
        condition_.notify_one();
    }
private:
    std::queue<T> queue_;
    std::mutex condition_mutex_;
    std::condition_variable condition_;
    bool notEmpty_;
};
#endif