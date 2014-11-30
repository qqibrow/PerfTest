#include <iostream>
#include <ctime>
#include <thread>
#include "countdown_latch.h"
#include "BlockingQueue.h"
#include "LockBlockingQueue.h"

#define COUNT (1000000000)

using namespace std;



// BlockingQueue and latch must be thread-safe.
class QueueTest {
public:
    explicit QueueTest(BlockingQueue<long>* blockingQueue): queue_(blockingQueue) {}
    void Reset(countdown_latch* latch) {
        value_ = 0;
        latch_ = latch;
    }

    long GetValue() const {
        return value_;
    }

    void Start(const long count) {
        long sequence = 0;
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

private:
    BlockingQueue<long>* queue_;
    countdown_latch* latch_;
    long value_;
    bool running_;

};

int main() {
    BlockingQueue<long>* blockingQueue = new LockBlockingQueue<long>();
    QueueTest queueTest(blockingQueue);
    countdown_latch latch(1);
    queueTest.Reset(&latch);

    long expect = 0;
    for(long i = 0; i < COUNT; ++i) {
        expect += 1;
        blockingQueue->put(1);
    }
    thread th(&QueueTest::Start, &queueTest, COUNT);
    std::clock_t c_start = std::clock();
    latch.wait();
    std::clock_t c_end = std::clock();
    printf("Expect %,d vs Actual %,d\n", expect, queueTest.GetValue());
    double miliseconds = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    long ops_per_seconds = (double)COUNT*1000 / miliseconds;
    printf("%,d ops per seconds\n", ops_per_seconds);
    th.join();
    delete blockingQueue;
    return 0;
}