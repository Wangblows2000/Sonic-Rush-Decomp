/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_CheckFallOffGround();
s32 NNS_SndPlayerStopSeq();
s32 Player_Action_Fall();
s32 Player_Action_LandOnGround();
s32 Player_Action_Grind();
s32 Player_SetAnimSpeedFromVelocity();
s32 Player_UpdateRunAnimation();
s32 Player_ApplyGroundInput();
extern u8 gGameState[];
extern u8 gCamera[];
s32 EffWaterRunSplash_Create();

void Player_State_WaterRun(u8 *p0) {
    s32 t0 = *(s16 *)((u8 *)p0 + 0x37c);
    s32 t1 = *(s16 *)((u8 *)p0 + 0x34);
    s32 v2 = t1;
    if (t1 < 0) {
        v2 = -v2;
    }
    if (v2 < t0) {
        s32 t3 = Player_CheckFallOffGround(p0, v2);
        if (t3 != 0) {
            NNS_SndPlayerStopSeq(((u8 *)p0 + 0x384), 0);
            *(u8 *)((u8 *)p0 + 0x1) = 0;
            s32 t4 = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
            *(u32 *)((u8 *)p0 + 0x58) = t4;
            Player_Action_Fall(p0, t4);
            return;
        }
    }
    if ((*(u16 *)((u8 *)p0 + 0x422) & 3) != 0) {
        if (*(u32 *)((u8 *)p0 + 0x340) != 0) {
            NNS_SndPlayerStopSeq(((u8 *)p0 + 0x384), 0);
            *(u8 *)((u8 *)p0 + 0x1) = 0;
            *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
            ((s32 (*)())*(u32 *)((u8 *)p0 + 0x340))(p0, *(u32 *)((u8 *)p0 + 0x340));
            return;
        }
    }
    s32 t5 = *(s16 *)((u8 *)p0 + 0x32a);
    if (t5 != 0) {
        *(u16 *)((u8 *)p0 + 0x32a) = (t5 - 0x1);
    } else {
        if ((*(u32 *)((u8 *)p0 + 0x58) & 0xd) != 0) {
            NNS_SndPlayerStopSeq(((u8 *)p0 + 0x384), 0);
            *(u8 *)((u8 *)p0 + 0x1) = 0;
            *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
            *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
            s32 t6 = *(s16 *)((u8 *)p0 + 0x34);
            *(u16 *)((u8 *)p0 + 0x2e) = t6;
            Player_Action_LandOnGround(p0, 0, t6);
            if (*(u32 *)((u8 *)p0 + 0x3f8) == 2) {
                Player_Action_Grind(p0);
                return;
            }
            ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
            return;
        }
    }
    Player_SetAnimSpeedFromVelocity(p0, *(s16 *)((u8 *)p0 + 0x34));
    Player_UpdateRunAnimation(p0);
    Player_ApplyGroundInput(p0);
    if ((*(u32 *)((u8 *)gGameState + 0x20) & 7) != 0) {
        return;
    }
    EffWaterRunSplash_Create(p0, 0, 0, *(u16 *)((u8 *)gCamera + 0x62));
}
