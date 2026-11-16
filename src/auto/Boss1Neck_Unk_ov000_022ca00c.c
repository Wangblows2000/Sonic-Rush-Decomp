/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 Boss1Neck_Unk_ov000_022c9dbc();

void Boss1Neck_Unk_ov000_022ca00c(s32 p0) {
    s32 t0 = *(u32 *)gPlayers;
    *(u16 *)((u8 *)p0 + 0x21a) = 0;
    Boss1Neck_Unk_ov000_022c9dbc(p0, *(u32 *)((u8 *)t0 + 0x10), 0x1000, 0x100, 0x20, 0x40, 0x400);
}
