#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono;
using namespace std;

//#define ALIGN_CACHE_LINE
#define MAX (1024 * 1024 * 32)
int main(int argc, char const* argv[]) {
#ifdef ALIGN_CACHE_LINE
    alignas(64) int x = 0;
    alignas(64) int y = 0;
#else
    int x = 0;
    int y = 0;
#endif
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
