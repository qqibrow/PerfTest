#ifndef QQIBROW_QUEUETEST_H_
#define QQIBROW_QUEUETEST_H_

#include "BlockingQueue.h"
#include "countdown_latch.h"

// BlockingQueue and latch must be thread-safe.
class QueueTest {
public:
    explicit QueueTest(BlockingQueue<long>* blockingQueue): queue_(blockingQueue) {}
    void RunTest(const long count);
private:
    void Reset(countdown_latch* latch);
    long GetValue() const;
    void Start(const long);
private:
    BlockingQueue<long>* queue_;
    countdown_latch* latch_;
    long value_;
    bool running_;
};

#endif