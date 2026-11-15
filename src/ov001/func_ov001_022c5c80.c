#include <nitro/types.h>

extern u32 data_022c4560;
void func_0205247c(u32 a, u32 b);

#pragma thumb on
void func_ov001_022c5c80(u32 a) {
    data_022c4560 = 0;
    func_0205247c(0, a);
}
#pragma thumb reset
