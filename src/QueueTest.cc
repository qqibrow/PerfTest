#include "QueueTest.h"

using namespace std;

QueueTest::~QueueTest() {
    if(queue_ != nullptr) {
        delete queue_;
    }
}

void QueueTest::Reset(countdown_latch* latch) {
    value_ = 0;
    latch_ = latch;
}

long QueueTest::GetValue() const {
    return value_;
}

