#ifndef QQIBROW_BLOCKINGQUEUE
#define QQIBROW_BLOCKINGQUEUE

template <typename T>
class BlockingQueue {
public:
    BlockingQueue() {}

    // Blocking call. Return until operation done.
    virtual T take() = 0;
    virtual void put(T value) = 0;

    // Non-blocking call. Immediately return whether operation succeed.
    virtual bool poll(T& value) = 0;
    virtual bool offer(const T& value) = 0;

    virtual std::string GetQueueName() const { return std::string("NULL");};
    virtual ~BlockingQueue() {}
};
#endif