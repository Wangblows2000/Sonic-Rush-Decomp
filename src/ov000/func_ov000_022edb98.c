#include <nitro/types.h>

s32 func_ov000_022edb98(s32 a, s32 b) {
    if (a & 3)
        return b + (((0x80 - b) << 10) >> 12);
    return (b * 0xf80) >> 12;
}
