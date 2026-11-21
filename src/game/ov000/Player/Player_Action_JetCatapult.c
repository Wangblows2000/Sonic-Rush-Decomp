/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Player_InitState();
s32 Player_SetAnimation();
void Player_State_JetCatapult();
s32 Sound_PlaySeqArcEx();
s32 AttackHitbox_Create();
s32 AttackHitbox_SetJetCatapultState();

void Player_Action_JetCatapult(u8 *p0, s32 p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x3e8);
    if (t0 == (s32)p1) {
        return;
    }
    Player_InitState(p0, p1, t0);
    *(u32 *)((u8 *)p0 + 0x3e8) = p1;
    Player_SetAnimation(p0, 3);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)Player_State_JetCatapult;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x200000);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0xf);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x100000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10000000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x2000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~1);
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x3e8) + 0x10);
    s32 t1 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x3e8) + 0x14) - 0xc00);
    *(u32 *)((u8 *)p0 + 0x14) = t1;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u8 *)((u8 *)p0 + 0x1) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x3f0) = 0;
    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x6d, t1);
    AttackHitbox_Create(p0, -32, -32, 0x20, 0x20, 0);
    AttackHitbox_SetJetCatapultState();
}
