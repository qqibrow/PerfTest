#include <iostream>
#include <ctime>
#include <thread>
#include <assert.h>

#include "QueueTest.h"

using namespace std;

void QueueTest::Reset(countdown_latch* latch) {
    value_ = 0;
    latch_ = latch;
}

long QueueTest::GetValue() const {
    return value_;
}

void QueueTest::Start(const long count) {
    long sequence = 1;
    running_ = true;
    while(running_) {
        long curr = queue_->take();
        this->value_ += curr;

        if(sequence++ == count) {
            latch_->count_down();
            running_ = false;
        }
    }
}

void QueueTest::RunTest(const long count) {
    countdown_latch latch(1);
    this->Reset(&latch);

    long expect = 0;
    for(long i = 0; i < count; ++i) {
        expect += 1;
        this->queue_->put(1);
    }
    thread th(&QueueTest::Start, this, count);
    std::clock_t c_start = std::clock();
    latch.wait();
    std::clock_t c_end = std::clock();
    printf("Test Report:\n");
    assert(expect == this->GetValue() ||
            printf("Expect is not equal to Actual. Expect %ld, Actual %ld.\n", expect, this->GetValue()));
    double miliseconds = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    printf("Eclipsed time: %lf ms.\n", miliseconds);
    long ops_per_seconds = (double)count*1000 / miliseconds;
    printf("%ld ops per seconds\n", ops_per_seconds);
    th.join();
    printf("Report finished.\n");
}