/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss7[];
s32 Boss7PartD5_SetAnimSpeedFromVelocity();
s32 EffFire01_3D_CreateFollowing();
s32 EffTrail_StartFollow();
void Boss7PartD5_MoveState_StartIdle();

void Boss7PartD5_MoveState_Roll(u8 *p0) {
    s32 t0 = *(u32 *)gBoss7;
    Boss7PartD5_SetAnimSpeedFromVelocity(p0, *(s16 *)((u8 *)p0 + 0x34), gBoss7);
    if (*(u32 *)((u8 *)p0 + 0x3d0) != 0) {
        if ((*(u32 *)((u8 *)t0 + 0x220) & 1) == 0) {
            EffFire01_3D_CreateFollowing(p0, 0, 0, 0);
        }
    }
    if (*(s16 *)((u8 *)p0 + 0x2e) != 0) {
        if ((*(u32 *)((u8 *)p0 + 0x58) & 4) == 0) {
            if ((*(u16 *)((u8 *)p0 + 0x3dc) & 0x40) == 0) {
                return;
            }
        }
    }
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
    *(u16 *)((u8 *)p0 + 0x2e) = ((s32)*(s16 *)((u8 *)p0 + 0x2e) >> 1);
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_StartIdle;
}
