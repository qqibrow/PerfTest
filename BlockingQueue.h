#ifndef QQIBROW_BLOCKINGQUEUE
#define QQIBROW_BLOCKINGQUEUE

template <typename T>
class BlockingQueue {
public:
    BlockingQueue() {}
    virtual T take() = 0;
    virtual void put(T value) = 0;

    // non-blocking call
    virtual bool poll(T& value) = 0;
    virtual bool offer(const T& value) = 0;
    virtual std::string GetQueueName() const { return std::string("NULL");}
    virtual ~BlockingQueue() {}
};
#endif