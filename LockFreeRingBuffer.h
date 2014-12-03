#include <boost/atomic.hpp>

// Single prudcer, sinlge consumer Ringbuffer

template<typename T, size_t Size = 100000000>
class LockFreeRingBuffer : public BlockingQueue<T> {
public:
    LockFreeRingBuffer() : head_(0), tail_(0) {}

    void put(T value)
    {
        size_t head = head_.load(boost::memory_order_relaxed);
        size_t next_head = next(head);

        // busy waiting until tail_ been consumed.
        while (next_head == tail_.load(boost::memory_order_acquire));

        ring_[head] = value;
        head_.store(next_head, boost::memory_order_release);
    }
    T take()
    {
        size_t tail = tail_.load(boost::memory_order_relaxed);

        // busy waiting until head advance with push.
        while (tail == head_.load(boost::memory_order_acquire));

        T value = ring_[tail];
        tail_.store(next(tail), boost::memory_order_release);
        return value;
    }
private:
    size_t next(size_t current)
    {
        return (current + 1) % Size;
    }
    T ring_[Size];
    boost::atomic<size_t> head_, tail_;
};