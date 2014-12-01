#include "LockBlockingQueue.h"
#include "LockFreeBlockingQueue.h"
#include "QueueTest.h"

using namespace std;

int main() {
    long count = 1000*1000*10;
    QueueTest lockQueueTest(new LockBlockingQueue<long>());
    lockQueueTest.RunTest(count, std::plus<long>());
    QueueTest lockFreeQueueTest(new LockFreeBlockingQueue<long>());
    lockFreeQueueTest.RunTest(count, std::plus<long>());
    return 0;
}
