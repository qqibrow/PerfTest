#include <boost/atomic.hpp>

// Single prudcer, sinlge consumer Ringbuffer

template<typename T>
class RingBufferV2 : public BlockingQueue<T> {
public:
    RingBufferV2() : head_(0), tail_(0) {
        const size_t size = 1024*1024;
        rightSize_ = findNextPositivePowerOfTwo(size);
        mask_ = rightSize_ - 1;
        ring_ = new T[rightSize_];
    }

    virtual ~RingBufferV2() {
        delete[] ring_;
    }

    void put(T value) {}
    T take() {}

    bool offer(const T& value)
    {
        const long currentTail = tail_.load(boost::memory_order_relaxed);
        const long wrapPoint = currentTail - rightSize_;

        if(headCache_.value <= wrapPoint) {
            headCache_.value = head_.load(boost::memory_order_acquire);
            if(headCache_.value <= wrapPoint) {
                return false;
            }
        }

        ring_[(long)currentTail & mask_] = value;
        tail_.store(currentTail + 1, boost::memory_order_release);
        return true;
    }
    bool poll(T& value)
    {
        const int currentHead = head_.load(boost::memory_order_relaxed);
        if(currentHead >= tailCache_.value) {
            tailCache_.value = tail_.load(boost::memory_order_acquire);
            if(currentHead >= tailCache_.value) {
                return false;
            }

        }
        int index = (int)currentHead & mask_;
        value = ring_[index];
        head_.store(currentHead + 1, boost::memory_order_release);
        return true;
    }
private:
    size_t findNextPositivePowerOfTwo(size_t s) {
        size_t res = 1;
        while(res < s) {
            res = res << 1;
        }
        return res;
    }
    T* ring_;
    size_t rightSize_;
    int mask_;

    class PaddingAtomicInt : public boost::atomic<long> {
    public:
        PaddingAtomicInt(long init) : boost::atomic<long>(init) {

        }
        volatile long p1, p2, p3, p4, p5, p6 = 7;
    };

    struct PaddingLong {
        PaddingLong() : value(0) {

        }
        long value, p1, p2, p3, p4, p5, p6;
    };


    PaddingAtomicInt head_, tail_;
    PaddingLong headCache_, tailCache_;
};