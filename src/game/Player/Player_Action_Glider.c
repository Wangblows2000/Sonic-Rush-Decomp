/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_ResetGimmickState();
s32 Player_SetAnimation();
void Player_State_Glider();
s32 Player_StopHighSpeed();
s32 Player_StopBoost();
s32 Sound_PlaySeqArcEx();

s32 Player_Action_Glider(u8 *p0, s32 p1, s32 p2, s32 p3) {
    if (*(u32 *)((u8 *)p0 + 0x3e8) == (s32)p1) {
        return (s32)p0;
    }
    Player_ResetGimmickState();
    *(u32 *)((u8 *)p0 + 0x3e8) = p1;
    Player_SetAnimation(p0, 0x2e);
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0x5c) = t0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Player_State_Glider;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x8000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    s32 t1 = (*(u32 *)((u8 *)p0 + 0x58) & ~0x40);
    *(u32 *)((u8 *)p0 + 0x58) = t1;
    Player_StopHighSpeed(p0, t1, t0);
    Player_StopBoost(p0);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x100000);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0xf);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~1);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x200);
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x3e8) + 0x10);
    *(u16 *)((u8 *)p0 + 0x2e) = 0x800;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u8 *)((u8 *)p0 + 0x1) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x1c) = (*(u32 *)((u8 *)p0 + 0x10) + ((s32)p2 << 8));
    s32 t2 = (*(u32 *)((u8 *)p0 + 0x14) + ((s32)p3 << 8));
    *(u32 *)((u8 *)p0 + 0x20) = t2;
    s32 t3 = Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x6d, t2);
    return t3;
}
