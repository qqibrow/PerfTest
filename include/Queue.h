#ifndef QQIBROW_BLOCKINGQUEUE
#define QQIBROW_BLOCKINGQUEUE

template <typename T>
class Queue {
public:
    Queue() {}

    // Blocking call. Return until operation done.
    virtual std::string GetName() { return "not set.";}
    // Take one element out of queue
    virtual T take() {
        T return_element;
        // Try to get one element until succeed.
        // TODO optimize using yield.
        while(!poll(return_element));
        return return_element;
    }

    // Put value into queue.
    virtual void put( T value) {
        while(!offer(value));
    }

    // Non-blocking call. Immediately return whether operation succeed.
    virtual bool poll(T& value) = 0;
    virtual bool offer(const T& value) = 0;

    virtual std::string GetQueueName() const { return std::string("NULL");};
    virtual ~Queue() {}
};
#endif