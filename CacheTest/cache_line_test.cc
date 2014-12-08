#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <assert.h>

using namespace std;
using namespace std::chrono;

#define LENGTH  (1024 * 1024 * 32)

void UpdateAtPosition(int* position) {
    for(int i = 0; i < LENGTH; ++i) {
        *position = *position + 3;
    }
}
int main(int argc, char * argv[])
{
    printf("%d", sizeof(int));
    if(argc < 3) {
        printf("Too few arguments\n");
        return -1;
    }
    int* arr = new int[LENGTH];

    auto t0 = high_resolution_clock::now();

    int num_thread = argc - 1;
    std::vector<thread> threads;
    for(int i = 0; i < num_thread; ++i) {
        const int pos = atoi(argv[1 + i]);
        assert(pos >= 0 && pos < LENGTH && "Pass in position must be in  the range");
        threads.push_back(thread(UpdateAtPosition, arr + pos));
    }

    for(auto& th: threads) {
        th.join();
    }

    auto t1 = high_resolution_clock::now();
    nanoseconds total_nanoseconds = std::chrono::duration_cast<nanoseconds>(t1 - t0);
    printf("Time exlipse %ld\n", total_nanoseconds.count());
    delete[] arr;
    return 0;
}
