//#include <gflags/gflags.h>
#include "LockBlockingQueue.h"
#include "LockFreeBlockingQueue.h"
#include "QueueTest.h"

//DEFINE_uint64(operator_count, 1000*1000, "");
using namespace std;


int main(int argc, char* argv[]) {
    //gflags::ParseCommandLineFlags(&argc, &argv, true);
    long FLAGS_operator_count = 1000*1000*10;

    printf("Pass in operator count: %lld.\n", FLAGS_operator_count);
    QueueTest lockQueueTest(new LockBlockingQueue<long>());
    lockQueueTest.RunTest(FLAGS_operator_count, std::plus<long>());
    QueueTest lockFreeQueueTest(new LockFreeBlockingQueue<long>());
    lockFreeQueueTest.RunTest(FLAGS_operator_count, std::plus<long>());
    return 0;
}
