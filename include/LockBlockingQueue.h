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
    virtual T take();
    virtual void put(T value);
    virtual ~LockBlockingQueue() {}
private:
    std::queue<T> queue_;
    std::mutex condition_mutex_;
    std::condition_variable condition_;
    bool notEmpty_;
};

#include "impl/LockBlockingQueue.defn.h"
#endif