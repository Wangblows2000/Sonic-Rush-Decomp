#include <nitro/types.h>

extern u32 data_ov000_02364d70[];

s32 FX_DivS32(s32 a, s32 b);

void Hud_FramesToTimeDigits(u32 *out, u32 t) {
    s16 sh;
    s16 i;
    u32 d;
    s32 q;

    if (t > 0x8c9f)
        t = 0x8c9f;
    i = 0;
    *out = 0;
    sh = 0x1c;
    while (1) {
        d = data_ov000_02364d70[i];
        if (t >= d) {
            q = FX_DivS32(t, d);
            *out |= q << sh;
            t -= q * d;
        }
        sh -= 4;
        if (sh == 4) {
            t = (t * 0x1b1) >> 8;
            if (t >= 100)
                t = 99;
        }
        if (sh == 0)
            break;
        i++;
    }
    *out |= t << sh;
}
