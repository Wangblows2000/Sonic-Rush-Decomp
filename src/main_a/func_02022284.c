#include <nitro/types.h>

extern u32 data_02084db8[];

void func_02022284(u32 idx, s32 x, s32 y, s32 z) {
    data_02084db8[idx] = ((x >> 3) & 0x3FF) | (((y >> 3) & 0x3FF) << 10) | (((z >> 3) & 0x3FF) << 20) | (idx << 30);
}
