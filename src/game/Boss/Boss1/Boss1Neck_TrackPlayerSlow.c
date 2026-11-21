/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 Boss1Neck_MoveTowardX();

void Boss1Neck_TrackPlayerSlow(s32 p0) {
    s32 t0 = *(u32 *)gPlayers;
    *(u16 *)((u8 *)p0 + 0x21a) = 0;
    Boss1Neck_MoveTowardX(p0, *(u32 *)((u8 *)t0 + 0x10), 0x800, 0x180, 0x40, 0x40, 0x500);
}
