#include <nitro/types.h>

void ExtraBoss_GetPlayer(void);
u8 *ExtraBoss_GetWork(void);
s32 FX_Div(s32 a, s32 b);

void ExtraBoss_StepXTowardCameraTarget(u8 *p) {
    s32 v, x, d;
    ExtraBoss_GetPlayer();
    d = (s32)(((s64)FX_Div(*(s32 *)(ExtraBoss_GetWork() + 0x31c) - 0x80000, 0x80000) * 0x53000 + 0x800) >> 12) + 0x100000;
    x = *(s32 *)(p + 0x18);
    v = d - x;
    if (v > 0) {
        if (v > 0x1000) {
            v = 0x1000;
        }
        if (x + v > 0x153000) {
            v = 0x153000 - x;
        }
    } else if (v < 0) {
        if (v < -0x1000) {
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
