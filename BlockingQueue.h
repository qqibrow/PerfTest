#ifndef QQIBROW_BLOCKINGQUEUE
#define QQIBROW_BLOCKINGQUEUE

template <typename T>
class BlockingQueue {
public:
    BlockingQueue() {}
    virtual T take() = 0;
    virtual void put(T value) = 0;
};
#endif