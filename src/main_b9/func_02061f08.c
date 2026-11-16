#include <nitro/types.h>

void MIi_CpuClear16(u16 v, void *dst, u32 size);

#pragma thumb on
static inline void Clear16_02061f08(void *p, u32 size) {
    vu16 z = 0;
    MIi_CpuClear16(z, p, size);
}

void func_02061f08(u8 x, u8 y, u8 w, u8 h, void *map) {
    u16 *m;
    u8 n;
    u8 j;
    u8 sx;

    m = map;
    if (x < 32) {
        if (x + w < 32)
            n = w;
        else
            n = 32 - x;
        for (j = y; j < y + h; j++) {
            Clear16_02061f08(&m[(j << 5) + x], n << 1);
        }
    }
    if (x + w > 32) {
        if (x > 32) {
            sx = x;
        } else {
            sx = 32;
            w -= 32 - x;
        }
        for (j = y; j < y + h; j++) {
            Clear16_02061f08((u16 *)((u8 *)m + 0x800) + ((j << 5) + (u8)(sx - 32)), w << 1);
        }
    }
}
#pragma thumb reset
