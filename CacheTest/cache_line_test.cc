#include <gflags/gflags.h>
#include <chrono>

#define LENGTH  (1024 * 1024 * 64)
DEFINE_int32(KK, 1, "The step of running loop.");
using namespace std;
using namespace std::chrono;
int main(int argc, char * argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    int* arr = new int[LENGTH];
    const int K = FLAGS_KK;
    auto t0 = high_resolution_clock::now();
    for (int i = 0; i < LENGTH; i += K) arr[i] *= 3;
    auto t1 = high_resolution_clock::now();
    nanoseconds total_nanoseconds = std::chrono::duration_cast<nanoseconds>(t1 - t0);
    printf("Time exlipse %ld\n", total_nanoseconds.count());
    delete[] arr;
    return 0;
}
