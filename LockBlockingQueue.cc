#include "LockBlockingQueue.h"
#include <assert.h>
template <typename T>
// This one must be the sam with the next take() method.!!!
T LockBlockingQueue<T>::take() {
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

template <>
long LockBlockingQueue<long>::take() {
    // First make sure queue is not empty.
    std::unique_lock<std::mutex> lock(condition_mutex_);
    while(!notEmpty_) {
        condition_.wait(lock);
    }

    if(!(notEmpty_ && !queue_.empty())) {
        printf("one condition is not satisfied.\n");
    }

        long curr = queue_.front();
        queue_.pop();

    if(queue_.empty()) {
        notEmpty_ = false;
    }

    lock.unlock();
    condition_.notify_one();
    return curr;
}

template <typename T>
void LockBlockingQueue<T>::put(T value) {
    {
        std::lock_guard<std::mutex> lk(condition_mutex_);
        queue_.push(value);
        notEmpty_ = true;
    }
    condition_.notify_one();
}

template <>
void LockBlockingQueue<long>::put(long value) {
    {
        std::lock_guard<std::mutex> lk(condition_mutex_);
        queue_.push(value);
        notEmpty_ = true;
    }
    condition_.notify_all();
}
