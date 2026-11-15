#include <nitro/types.h>

extern s32 data_0208f7e8;
extern u32 data_0208f7f0[];

u32 func_02033db0(void) {
    s32 n = data_0208f7e8;
    if (n >= 16) return 0;
    data_0208f7e8 = n + 1;
    return data_0208f7f0[n];
}
