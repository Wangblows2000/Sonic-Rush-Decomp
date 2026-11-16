#include <nitro/types.h>

void Player_SetAnimation(u8 *w, s32 a);
void Player_SetRunAnimationForSpeed(u8 *w);
void Player_State_GroundMove(void);
void Player_ApplyGroundInput(u8 *w);

void Player_Action_GroundMove(u8 *w) {
    if (((*(u16 *)(w + 0x5c) & 1) && (*(u16 *)(w + 0x420) & 0x10)) || (!(*(u16 *)(w + 0x5c) & 1) && (*(u16 *)(w + 0x420) & 0x20)))
        Player_SetAnimation(w, 0);
    else
        Player_SetRunAnimationForSpeed(w);
    *(u32 *)(w + 0x58) &= ~0x10;
    *(void **)(w + 0x68) = Player_State_GroundMove;
    if (*(s16 *)(w + 0x328) == 0) {
        if (*(u16 *)(w + 0x420) & 0x20)
            *(u32 *)(w + 0x32c) |= 1;
        else if (*(u16 *)(w + 0x420) & 0x10)
            *(u32 *)(w + 0x32c) &= ~1;
    }
    Player_ApplyGroundInput(w);
}
