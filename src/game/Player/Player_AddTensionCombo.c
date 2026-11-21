/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_AddTension();

void Player_AddTensionCombo(s32 p0, s32 p1) {
    s32 t0 = *(s16 *)((u8 *)p0 + 0x3b8);
    if (t0 == 0) {
        *(u16 *)((u8 *)p0 + 0x3ba) = 0;
    }
    s32 t1 = ((s32)((((s32)(s32)p1 >> 1) * *(s16 *)((u8 *)p0 + 0x3ba) + (s32)p1) << 0x10) >> 0x10);
    s32 v2 = t1;
    if (t1 < 0) {
        v2 = 0;
    } else {
        s32 v3 = v2;
        if (v2 > 0x12bf) {
            v3 = 0x12bf;
        }
        v2 = v3;
    }
    Player_AddTension(p0, ((s32)(v2 << 0x10) >> 0x10), ((s32)(s32)p1 >> 1));
    *(u16 *)((u8 *)p0 + 0x3b8) = 0x60;
    *(u16 *)((u8 *)p0 + 0x3ba) = (*(s16 *)((u8 *)p0 + 0x3ba) + 0x1);
}
