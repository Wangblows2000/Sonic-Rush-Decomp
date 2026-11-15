#include <nitro/types.h>

void func_ov001_02300c94(void);
void func_ov001_022ffce8(void);

void func_ov001_022eec34(u8 *p) {
    s32 x, v, d;
    func_ov001_02300c94();
    func_ov001_022ffce8();
    x = *(s32 *)(p + 0x18);
    d = 0x153000 - x;
    v = d;
    if (d > 0) {
        if (d > 0x1000) {
            v = 0x1000;
        }
        if (x + v > 0x153000) {
            v = d;
        }
    } else if (d < 0) {
        if (d < -0x1000) {
            v = -0x1000;
        }
        if (x + v < 0x100000) {
            v = 0x100000 - x;
        }
    }
    *(s32 *)(p + 0x50) = v;
    if (*(s32 *)(p + 0x1c) < 0x65000) {
        *(s32 *)(p + 0x54) = 0x1000;
    } else if (*(s32 *)(p + 0x1c) > 0x111000) {
        *(s32 *)(p + 0x54) = -0x1000;
    }
}
