#include <nitro/types.h>

void Player_State_Hurt(void);
s32 Player_InitState();
void Player_SetAnimation(u8 *w, s32 a);

void Player_Action_KnockbackIntangible(u8 *w, s16 vx, s16 vy) {
    Player_InitState(w);
    Player_SetAnimation(w, 0xf);
    *(s16 *)(w + 0x2e) = vx;
    *(s16 *)(w + 0x30) = vy;
    *(s16 *)(w + 0x34) = 0;
    if (vx < 0)
        *(u16 *)(w + 0x5c) &= ~1;
    else
        *(u16 *)(w + 0x5c) |= 1;
    *(u32 *)(w + 0x54) |= 2;
    *(u32 *)(w + 0x58) |= 0x10;
    *(u32 *)(w + 0x58) |= 0x8000;
    *(u32 *)(w + 0x58) &= ~0x80000;
    *(u32 *)(w + 0x58) &= ~1;
    *(void **)(w + 0x68) = Player_State_Hurt;
}
