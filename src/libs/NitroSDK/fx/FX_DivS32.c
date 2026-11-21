#include <nitro/types.h>

s32 FX_DivS32(s32 n, s32 d) {
    *(vu16 *)0x4000280 = 0;
    *(vu32 *)0x4000290 = n;
    *(volatile u64 *)0x4000298 = (u64)(u32)d;
    while (*(vu16 *)0x4000280 & 0x8000) {}
    return *(vs32 *)0x40002a0;
}
