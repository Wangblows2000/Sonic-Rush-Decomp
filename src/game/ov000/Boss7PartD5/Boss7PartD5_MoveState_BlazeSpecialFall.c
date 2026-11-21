/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss7[];
s32 Boss7_SetRivalAnimation();
void Boss7PartD5_MoveState_StartIdle();

void Boss7PartD5_MoveState_BlazeSpecialFall(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x58) & 1) == 0) {
        return;
    }
    s32 t0 = *(u32 *)gBoss7;
    *(u32 *)((u8 *)t0 + 0x210) = 1;
    Boss7_SetRivalAnimation(p0, 0x31, t0, 1);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_StartIdle;
}
