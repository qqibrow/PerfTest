#include "LockBlockingQueue.h"
#include "LockFreeBlockingQueue.h"
#include "QueueTest.h"

using namespace std;

int main() {
    int count = 1000000;
    QueueTest lockQueueTest(new LockBlockingQueue<long>());
    lockQueueTest.RunTest(count);
    QueueTest lockFreeQueueTest(new LockFreeBlockingQueue<long>());
    lockFreeQueueTest.RunTest(count);
    return 0;
}