/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_CheckFallOffGround();
s32 Player_LoadPhysicsParams();
s32 Player_Action_Fall();
s32 Player_ApplyRollFriction();
s32 Player_SetAnimSpeedFromVelocity();
extern u8 gObjManagerFlags[];
extern u8 gGameState[];
s32 EffBlazeSpinFlame_Spawn();

void Player_State_Roll(u8 *p0) {
    s32 t0 = Player_CheckFallOffGround();
    if (t0 != 0) {
        Player_LoadPhysicsParams(p0);
        Player_Action_Fall(p0);
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x422) & 3) != 0) {
        if (*(u32 *)((u8 *)p0 + 0x340) != 0) {
            s32 t1 = *(s16 *)((u8 *)p0 + 0x352);
            s32 v2 = t1;
            if (t1 >= 0x14) {
                *(u16 *)((u8 *)p0 + 0x352) = 0x14;
                v2 = 0x14;
            }
            Player_LoadPhysicsParams(p0, v2);
            ((s32 (*)())*(u32 *)((u8 *)p0 + 0x340))(p0, *(u32 *)((u8 *)p0 + 0x340));
            return;
        }
    }
    Player_ApplyRollFriction(p0);
    s32 t3 = *(s16 *)((u8 *)p0 + 0x34);
    if (t3 < 0x80) {
        if (t3 > -128) {
            *(u16 *)((u8 *)p0 + 0x34) = 0;
            Player_LoadPhysicsParams(p0, 0);
            ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
            return;
        }
    }
    Player_SetAnimSpeedFromVelocity(p0);
    if ((*(u32 *)gObjManagerFlags & 0x100) != 0) {
        return;
    }
    if (*(u8 *)((u8 *)p0 + 0x324) != 1) {
        return;
    }
    if ((*(u32 *)((u8 *)gGameState + 0x18) & 1) == 0) {
        return;
    }
    if ((*(u32 *)((u8 *)gGameState + 0x20) & 7) != 0) {
        return;
    }
    EffBlazeSpinFlame_Spawn(p0, (*(u32 *)((u8 *)gGameState + 0x18) & 1));
}
