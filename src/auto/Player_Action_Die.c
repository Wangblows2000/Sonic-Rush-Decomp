/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_InitState();
s32 Player_SetAnimation();
s32 ModelPalette_ShiftColor();
s32 Player_StopHighSpeed();
s32 Player_StopBoost();
void Player_State_Die();
s32 ScreenShake_Start();
s32 Sound_PlaySeqArcEx();

void Player_Action_Die(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x400) != 0) {
        return;
    }
    Player_InitState(p0, (*(u32 *)((u8 *)p0 + 0x32c) & 0x400));
    Player_SetAnimation(p0, 0x11);
    *(u16 *)((u8 *)p0 + 0x3b6) = 0;
    *(u16 *)((u8 *)p0 + 0x3b4) = 0;
    ModelPalette_ShiftColor(((u8 *)p0 + 0x414), 0, 0, 0);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x40000);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~1);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) | 0x200);
    *(u32 *)((u8 *)p0 + 0x58) = t0;
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    s32 t1 = -*(s16 *)((u8 *)p0 + 0x370);
    *(u16 *)((u8 *)p0 + 0x30) = t1;
    Player_StopHighSpeed(p0, t1, 0, t0);
    Player_StopBoost(p0);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x30000000);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Player_State_Die;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u16 *)((u8 *)p0 + 0x3ca) = 0;
    s32 t2 = (*(u32 *)((u8 *)p0 + 0x32c) | 0x400);
    *(u32 *)((u8 *)p0 + 0x32c) = t2;
    ScreenShake_Start(4, t2);
    if ((*(u32 *)((u8 *)p0 + 0x32c) & 0x4000000) != 0) {
        Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x52);
        return;
    }
    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 6);
}
