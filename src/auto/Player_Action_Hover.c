/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_SetAnimation();
s32 EffBoostJet_Spawn();
void Player_State_Hover();
s32 Sound_PlaySeqArcEx();

void Player_Action_Hover(u8 *p0) {
    if (*(s16 *)((u8 *)p0 + 0x3e0) == 0) {
        return;
    }
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x5c) | 0x400);
    *(u16 *)((u8 *)p0 + 0x5c) = t0;
    Player_SetAnimation(p0, 0x34, t0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~3);
    if ((*(u32 *)((u8 *)p0 + 0x330) & 0x1000) == 0) {
        EffBoostJet_Spawn(p0);
    }
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    if ((*(u32 *)((u8 *)p0 + 0x330) & 0x1000) != 0) {
        *(u16 *)((u8 *)p0 + 0x32) = 0;
    }
    *(u16 *)((u8 *)p0 + 0x40) = 8;
    *(u16 *)((u8 *)p0 + 0x42) = 0x220;
    *(u16 *)((u8 *)p0 + 0x356) = 0x30;
    *(u16 *)((u8 *)p0 + 0x35a) = 0x60;
    *(u16 *)((u8 *)p0 + 0x366) = 0x38;
    *(u16 *)((u8 *)p0 + 0x36a) = 0x60;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Player_State_Hover;
    s32 t1 = *(s16 *)((u8 *)p0 + 0x2a);
    if (t1 > 0x300) {
        *(u16 *)((u8 *)p0 + 0x62) = 0x44;
    }
    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x15);
}
