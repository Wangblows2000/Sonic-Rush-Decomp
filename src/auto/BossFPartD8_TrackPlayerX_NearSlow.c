/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 BossFPartD8_MoveTowardX();

void BossFPartD8_TrackPlayerX_NearSlow(s32 p0) {
    BossFPartD8_MoveTowardX(p0, *(u32 *)((u8 *)*(u32 *)gPlayers + 0x10), 0x2000, 0x100, 0x20, 0x40, 0x700);
}
