#include <nitro/types.h>

void SpecialStageGimmick_Destroy(void *p);

void func_ov001_0230e634(s32 *p) {
    p[0x2c / 4] += p[0x38 / 4];
    p[0x30 / 4] += p[0x3c / 4];
    p[0x34 / 4] += p[0x40 / 4];
    p[0x20 / 4] += p[0x2c / 4];
    p[0x24 / 4] += p[0x30 / 4];
    p[0x28 / 4] += p[0x34 / 4];
    if (p[0x28 / 4] > 0xf000) {
        SpecialStageGimmick_Destroy(p);
    }
}
