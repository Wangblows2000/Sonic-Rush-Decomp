/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void EnemyFlapperGroup_State_FireShot_ov000_02300338();

void EnemyFlapperGroup_Action_FireShot_ov000_02300438(u8 *p0) {
    StageObj_SetAnimation(p0, 3);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyFlapperGroup_State_FireShot_ov000_02300338;
    *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~1);
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0x60;
}
