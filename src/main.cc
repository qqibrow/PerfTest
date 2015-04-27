#include <thread>
#include <gflags/gflags.h>
#include <chrono>
#include <iostream>


#include "LockBlockingQueue.h"
#include "LockFreeBlockingQueue.h"
#include "LockFreeRingBuffer.h"
#include "LockFreeRingBufferV2.h"
#include "LockFreeRingBufferV3.h"

#define TEST_VALUE 5
#define RUN_TIMES 5

DEFINE_uint64(operator_count, 1000*1000, "The time of simple operator to run in test.");
using namespace std;

void TestInBatch(Queue<int>* queue);

int main(int argc, char* argv[]) {

    gflags::ParseCommandLineFlags(&argc, &argv, true);
    printf("Pass in operator count: %lld.\n", FLAGS_operator_count);

    TestInBatch(new LockFreeBlockingQueue<int>());
    TestInBatch(new LockFreeRingBuffer<int>());
    TestInBatch(new LockFreeRingBufferV2<int>());
    TestInBatch(new LockFreeRingBufferV3<int>());

    return 0;

}

void RunPerfTest(Queue<int>* queue, const long count);
void TestInBatch(Queue<int>* queue) {
    cout << queue->GetName() << endl;
    for(int i = 0; i < RUN_TIMES; ++i) {
        RunPerfTest(queue, FLAGS_operator_count);
    }
}

void Producer(Queue<int>* queue, const long count) {
    long i = count;
    do {
        while(!queue->offer(TEST_VALUE)) {
            std::this_thread::yield();
        }
    } while( 0 != --i);
}

void RunPerfTest(Queue<int>* queue, const long count) {
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;
    using std::chrono::nanoseconds;


    // do something here.
    std::thread producer(Producer, queue, count);
    int result;
    long i = count;
    auto t0 = high_resolution_clock::now();
    do {
        while(!queue->poll(result)) {
            std::this_thread::yield();
        }
    } while( 0 != --i);
    producer.join();
    auto t1 = high_resolution_clock::now();
    nanoseconds total_nanoseconds = std::chrono::duration_cast<nanoseconds>(t1 - t0);
    long long ops = count * 1000 * 1000 * 1000 / total_nanoseconds.count();
    printf("%ld - ops/sec=%lld - %s result=%d\n",count, ops, queue->GetQueueName().c_str(), result);
}


