#include <nitro/types.h>

u32 func_0203422c(u16 c);

#pragma thumb on

void func_ov000_0235fbe8(u32 *out, u16 *src, s32 n) {
    s32 i;

    for (i = 0; i < n && i < 6; i++) {
        out[i] = func_0203422c(src[i]);
    }
    if ((u32)n < 7) {
        for (; i < 7; i++) {
            out[i] = func_0203422c(0x20);
        }
    } else {
        out[6] = func_0203422c(0x2026);
    }
}

#pragma thumb reset
