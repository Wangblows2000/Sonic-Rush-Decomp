#include <nitro/types.h>

void FX_InvAsync(s32 d) {
    *(vu16 *)0x4000280 = 1;
    *(volatile u64 *)0x4000290 = (u64)0x1000 << 32;
    *(volatile u64 *)0x4000298 = (u64)(u32)d;
}
