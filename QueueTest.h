#ifndef QQIBROW_QUEUETEST_H_
#define QQIBROW_QUEUETEST_H_

#include <iostream>
#include <ctime>
#include <thread>
#include <assert.h>

#include "BlockingQueue.h"
#include "countdown_latch.h"


// BlockingQueue and latch must be thread-safe.
class QueueTest {
public:
    explicit QueueTest(BlockingQueue<long>* blockingQueue): queue_(blockingQueue) {}
    virtual ~QueueTest();

    template <typename Ops>
    void RunTest(const long count, Ops ops);
private:
    void Reset(countdown_latch* latch);
    long GetValue() const;

    template <typename Ops>
    void Start(const long, Ops ops);
private:
    BlockingQueue<long>* queue_;
    countdown_latch* latch_;
    long value_;
    bool running_;
};

template <typename Ops>
void QueueTest::Start(const long count, Ops ops) {
    long sequence = 1;
    running_ = true;
    while(running_) {
        long curr = queue_->take();
        this->value_ = ops(this->value_, curr);

        if(sequence++ == count) {
            latch_->count_down();
            running_ = false;
        }
    }
}

template <typename Ops>
void QueueTest::RunTest(const long count, Ops ops) {
    countdown_latch latch(1);
    this->Reset(&latch);

    long expect = 0;
    std::thread producer([&]() {
        long local_expect = 0;
        for(long i = 0; i < count; ++i) {
            long curr = i;
            local_expect = ops(local_expect, curr);
            queue_->put(curr);
        }
        expect = local_expect;
    });

    std::thread th(&QueueTest::Start<Ops>, this, count, ops);
    std::clock_t c_start = std::clock();
    latch.wait();
    std::clock_t c_end = std::clock();
    printf("Test Report:\n");
    assert(expect == this->GetValue() ||
            printf("Expect is not equal to Actual. Expect %ld, Actual %ld.\n", expect, this->GetValue()));
    double miliseconds = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    printf("Eclipsed time: %lf ms.\n", miliseconds);
    long ops_per_seconds = (double)count*1000 / miliseconds;
    printf("%'ld ops per seconds\n", ops_per_seconds);
    th.join();
    producer.join();
    printf("Report finished.\n");
}
#endif
