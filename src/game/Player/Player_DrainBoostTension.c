/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_AddTension();

void Player_DrainBoostTension(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x32c);
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x80) == 0) {
        return;
    }
    if (*(s16 *)((u8 *)p0 + 0x3b6) != 0) {
        return;
    }
    if (*(u32 *)((u8 *)p0 + 0x3e8) != 0) {
        if ((*(u32 *)((u8 *)p0 + 0x330) & 8) == 0) {
            return;
        }
    }
    if ((t0 & 0x100000) != 0) {
        return;
    }
    Player_AddTension(p0, -8, t0);
}
