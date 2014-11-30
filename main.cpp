#include "LockBlockingQueue.h"
#include "LockFreeBlockingQueue.h"
#include "QueueTest.h"

using namespace std;

int main() {
    int count = 100;
    QueueTest lockQueueTest(new LockBlockingQueue<long>());
    lockQueueTest.RunTest(count);
    QueueTest lockFreeQueueTest(new LockBlockingQueue<long>());
    lockFreeQueueTest.RunTest(count);
    return 0;
}