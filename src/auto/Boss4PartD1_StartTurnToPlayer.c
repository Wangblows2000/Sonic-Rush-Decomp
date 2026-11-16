/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss4PartD1_GetPlayerDirection();
void Boss4PartD1_SubState_TurnToPlayer();

void Boss4PartD1_StartTurnToPlayer(u8 *p0) {
    s32 t0 = Boss4PartD1_GetPlayerDirection();
    *(u32 *)((u8 *)p0 + 0x348) = t0;
    *(u32 *)((u8 *)p0 + 0x3c0) = (u32)Boss4PartD1_SubState_TurnToPlayer;
    *(u32 *)((u8 *)p0 + 0x3c4) = 0;
    *(u32 *)((u8 *)p0 + 0x3a8) = 0;
    *(u32 *)((u8 *)p0 + 0x3ac) = 0;
    *(u32 *)((u8 *)p0 + 0x3b0) = 0;
    *(u32 *)((u8 *)p0 + 0x3b4) = 0;
    *(u32 *)((u8 *)p0 + 0x3b8) = 0;
    *(u32 *)((u8 *)p0 + 0x3bc) = 0;
}
