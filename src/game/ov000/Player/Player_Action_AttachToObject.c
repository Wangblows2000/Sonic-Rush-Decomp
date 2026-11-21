/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_ResetGimmickState();
s32 Player_SetAnimation();
void Player_State_AttachedToObject();
s32 Sound_PlaySeqArcEx();

s32 Player_Action_AttachToObject(u8 *p0, s32 p1, s32 p2, s32 p3) {
    if (*(u32 *)((u8 *)p0 + 0x3e8) == (s32)p1) {
        return (s32)p0;
    }
    Player_ResetGimmickState();
    *(u32 *)((u8 *)p0 + 0x3e8) = p1;
    Player_SetAnimation(p0, 0x2c);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Player_State_AttachedToObject;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x8000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x2000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x40);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x100000);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0xf);
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u8 *)((u8 *)p0 + 0x1) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x3f0) = ((s32)p2 << 8);
    *(u32 *)((u8 *)p0 + 0x3f4) = ((s32)p3 << 8);
    *(u32 *)((u8 *)p0 + 0x1c) = *(u32 *)((u8 *)p0 + 0x10);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x14);
    *(u32 *)((u8 *)p0 + 0x20) = t0;
    s32 t1 = Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x6d, t0);
    return t1;
}
