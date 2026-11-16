#include <nitro/types.h>

u32 func_0203422c(u16 id);

#pragma thumb on
void func_ov001_022ec980(u32 *out, u16 *ids, s32 n) {
    s32 i = 0;

    while (i < n && i < 6) {
        out[i] = func_0203422c(*ids);
        ids++;
        i++;
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
