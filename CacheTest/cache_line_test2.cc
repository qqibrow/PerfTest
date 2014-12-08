#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono;
using namespace std;

#define MAX (1024 * 1024 * 32)
int main(int argc, char const* argv[]) {
    int x = 0;
    int y = 0;
    auto t0 = high_resolution_clock::now();
    std::thread th([&x]() {
        for (int i = 0; i < MAX; ++i) {
            ++x;
        }
    });
    for (int i = 0; i < MAX; ++i) {
        ++y;
    }

    th.join();
    auto t1 = high_resolution_clock::now();
    nanoseconds total_nanoseconds = std::chrono::duration_cast<nanoseconds>(t1 - t0);
    printf("Time exlipse %ld\n", total_nanoseconds.count());
    return 0;
}
