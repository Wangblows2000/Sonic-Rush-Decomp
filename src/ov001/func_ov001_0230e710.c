#include <nitro/types.h>

void func_ov001_0230e6c0(void);

void func_ov001_0230e710(s32 *p) {
    p[0x20 / 4] += p[0x2c / 4];
    p[0x24 / 4] += p[0x30 / 4];
    p[0x28 / 4] += p[0x34 / 4];
    if (p[0xd0 / 4] < 0x14) {
        return;
    }
    p[0x2c / 4] *= -1;
    p[0x30 / 4] *= -1;
    p[0x34 / 4] *= -1;
    p[0xcc / 4] = (s32)func_ov001_0230e6c0;
    p[0xd0 / 4] = 0;
}
