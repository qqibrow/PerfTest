#include "countdown_latch.h"

using namespace std;

countdown_latch::countdown_latch(unsigned int count) : count_(count) {
}

countdown_latch::~countdown_latch() {
}

void countdown_latch::wait() {
    unique_lock<mutex> lock(condition_mutex_);
    while(count_ > 0)
        condition_.wait(lock);
}

void countdown_latch::count_down() {
    lock_guard<mutex> lock(condition_mutex_);
    if(count_ <= 0) {
        throw std::system_error(std::make_error_code(std::errc::invalid_argument));
    }
    if(--count_ == 0) {
        condition_.notify_all();
    }
}