/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss7[];
s32 Boss7_SetRivalAnimation();
s32 Boss7_SetCollidersEnabled();
s32 EffTrail_StartFollow();
extern u8 gBossStatus[];
void Boss7PartD5_MoveState_Idle();

void Boss7PartD5_MoveState_StartIdle(u8 *p0) {
    s32 t0 = *(u32 *)gBoss7;
    *(u32 *)((u8 *)t0 + 0x210) = 1;
    Boss7_SetRivalAnimation(p0, 0x3c, 1);
    Boss7_SetCollidersEnabled(p0, 1, 0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x480);
    s32 v2 = t1;
    if (t1 != 0) {
        EffTrail_StartFollow(v2);
        *(u32 *)((u8 *)p0 + 0x480) = 0;
        v2 = 0;
    }
    s32 t3 = *(u32 *)((u8 *)p0 + 0x488);
    if (t3 != 0) {
        *(u32 *)((u8 *)t3 + 0x54) = (*(u32 *)((u8 *)t3 + 0x54) | 8);
        *(u32 *)((u8 *)p0 + 0x488) = 0;
    }
    *(u32 *)((u8 *)t0 + 0x20c) = (*(u32 *)((u8 *)t0 + 0x20c) & ~0x10);
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x24;
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_Idle;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x440))(p0, *(u32 *)((u8 *)p0 + 0x440), 0x24);
}
