#include <nitro/types.h>

s32 func_020021f0(s32 a);
u16 func_020027a4(s32 a, s32 b);

u16 func_ov001_02312ac8(s32 x) {
    s32 r = func_020021f0(0x38400 - (s32)(((s64)(x >> 2) * (x >> 2) + 0x800) >> 12));
    if ((r << 2) == 0) {
        if (x > 0) {
            return 0x4000;
        }
        if (x < 0) {
            return 0xc000;
        }
        return 0;
    }
    return func_020027a4(x, r << 2);
}
