#include <gflags/gflags.h>

//#define LENGTH  (64 * 1024 * 1024)
DEFINE_int32(KK, 1, "The step of running loop.");

int main(int argc, char * argv[])
{
    int length = 1024*1024;
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    int arr[length] = {1};
    const int K = FLAGS_KK;
    for (int i = 0; i < length; i += K) arr[i] *= 3;
    return 0;
}
