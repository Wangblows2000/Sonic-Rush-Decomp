/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_SetAnimation();
s32 Player_Action_Roll();
s32 Sound_PlaySeqArcEx();
s32 NNS_SndPlayerStopSeq();
s32 ObjSpd_UpSet();
s32 EffSpinSmoke2_Spawn();
s32 Player_SetAnimSpeedFromVelocity();
s32 ObjSpd_DownSet();
s32 EffSpinSmoke1_Spawn();

void Player_State_SpinDash(u8 *p0) {
    if (*(s16 *)((u8 *)p0 + 0x328) == 0x33) {
        if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) != 0) {
            Player_SetAnimation(p0, 0x32);
            *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
        }
    }
    s32 t0 = *(u16 *)((u8 *)p0 + 0x420);
    if ((*(u16 *)((u8 *)p0 + 0x420) & 0x80) == 0) {
        *(u16 *)((u8 *)p0 + 0x352) = 0x48;
        *(u8 *)((u8 *)p0 + 0x3d1) = 8;
        *(u16 *)((u8 *)p0 + 0x34) = ((((s32)*(s16 *)((u8 *)p0 + 0x32a) >> 8) << 7) + 0x800);
        s32 t1 = (*(u16 *)((u8 *)p0 + 0x5c) & 1);
        if (t1 != 0) {
            *(u16 *)((u8 *)p0 + 0x34) = -*(s16 *)((u8 *)p0 + 0x34);
        }
        s32 t2 = (*(u32 *)((u8 *)p0 + 0x58) | 0x4000);
        *(u32 *)((u8 *)p0 + 0x58) = t2;
        Player_Action_Roll(p0, t2, t0);
        if (*(u8 *)((u8 *)p0 + 0x324) != 1) {
            Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 1);
            return;
        }
        Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x12);
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x422) & 3) != 0) {
        NNS_SndPlayerStopSeq(((u8 *)p0 + 0x384), 0);
        if (*(u8 *)((u8 *)p0 + 0x324) != 1) {
            Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0);
        } else {
            Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x11);
        }
        Player_SetAnimation(p0, 0x33);
        s32 t3 = ObjSpd_UpSet(*(s16 *)((u8 *)p0 + 0x32a), *(s16 *)((u8 *)p0 + 0x35e), *(s16 *)((u8 *)p0 + 0x360));
        *(u16 *)((u8 *)p0 + 0x32a) = t3;
        if ((*(u32 *)((u8 *)p0 + 0x330) & 0x1000) == 0) {
            if ((*(u32 *)((u8 *)p0 + 0x32c) & 1) == 0) {
                EffSpinSmoke2_Spawn(p0, ((u8 *)p0 + 0x300));
            }
        }
        *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 1);
        return;
    }
    if ((t0 & 0x40) != 0) {
        ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
        return;
    }
    Player_SetAnimSpeedFromVelocity(p0, ((s32)(((((s32)*(s16 *)((u8 *)p0 + 0x32a) >> 8) << 7) + 0x800) << 0x10) >> 0x10));
    if (*(s16 *)((u8 *)p0 + 0x32a) < ((s32)*(s16 *)((u8 *)p0 + 0x35c) >> 1)) {
        s32 t4 = (*(u32 *)((u8 *)p0 + 0x32c) & 1);
        if (t4 != 0) {
            *(u32 *)((u8 *)p0 + 0xc) = 5;
            *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~1);
        }
    }
    s32 t5 = ObjSpd_DownSet(*(s16 *)((u8 *)p0 + 0x32a), ((s32)*(s16 *)((u8 *)p0 + 0x32a) >> 5));
    *(u16 *)((u8 *)p0 + 0x32a) = t5;
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 1) != 0) {
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0xc) & 0xf) == 0) {
        EffSpinSmoke1_Spawn(p0, ((u8 *)p0 + 0x300));
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x1);
}
