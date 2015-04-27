#ifndef LOCK_FREE
#define LOCK_FREE
#include <atomic>

// Single prudcer, sinlge consumer Ringbuffer

template<typename T, size_t Size = 100000000>
class LockFreeRingBuffer : public Queue<T> {
public:
    LockFreeRingBuffer() : head_(0), tail_(0) {}

    void put(T value) {}
    T take() {}

    bool offer(const T& value)
    {
        size_t head = head_.load(std::memory_order_relaxed);
        size_t next_head = next(head);

        // busy waiting until tail_ been consumed.
        if (next_head == tail_.load(std::memory_order_acquire))
            return false;

        ring_[head] = value;
        head_.store(next_head, std::memory_order_release);
        return true;
    }
    bool poll(T& value)
    {
        size_t tail = tail_.load(std::memory_order_relaxed);

        // busy waiting until head advance with push.
        if (tail == head_.load(std::memory_order_acquire))
            return false;

        value = ring_[tail];
        tail_.store(next(tail), std::memory_order_release);
        return true;
    }
private:
    size_t next(size_t current)
    {
        return (current + 1) % Size;
    }
    T ring_[Size];
    std::atomic<size_t> head_, tail_;
};

#endif