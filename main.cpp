#include <gflags/gflags.h>
#include <chrono>

#include "LockBlockingQueue.h"
#include "LockFreeBlockingQueue.h"
#include "LockFreeRingBuffer.h"
#include "RingBufferV2.h"
#include "QueueTest.h"

#define TEST_VALUE 5

DEFINE_uint64(operator_count, 1000*1000, "The time of simple operator to run in test.");
using namespace std;

void RunPerfTest(BlockingQueue<int>* queue, const long count);

int main(int argc, char* argv[]) {

    gflags::ParseCommandLineFlags(&argc, &argv, true);
    /*
    printf("Pass in operator count: %lld.\n", FLAGS_operator_count);
    QueueTest lockQueueTest(new LockBlockingQueue<long>());
    lockQueueTest.RunTest(FLAGS_operator_count, std::minus<long>());
    QueueTest lockFreeQueueTest(new LockFreeBlockingQueue<long>());
    lockFreeQueueTest.RunTest(FLAGS_operator_count, std::minus<long>());
    QueueTest ringBufferTest(new LockFreeBlockingQueue<long>());
    ringBufferTest.RunTest(FLAGS_operator_count, std::minus<long>());
    */
    /*
    printf("LockFreeRingBuffer\n");
    BlockingQueue<int>* q = new LockFreeRingBuffer<int>();
    for(int i = 0; i < 10; ++i) {
        RunPerfTest(q, FLAGS_operator_count);
    }
    */
    printf("RingbufferV2\n");
    BlockingQueue<int>* q2 = new RingBufferV2<int>();
    for(int i = 0; i < 10; ++i) {
        RunPerfTest(q2, FLAGS_operator_count);
    }

    return 0;

}

void Producer(BlockingQueue<int>* queue, const long count) {
    long i = count;
    do {
        while(!queue->offer(TEST_VALUE)) {
            std::this_thread::yield();
        }
    } while( 0 != --i);
}

void RunPerfTest(BlockingQueue<int>* queue, const long count) {
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


