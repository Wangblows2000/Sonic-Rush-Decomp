#include <nitro/types.h>

void Player_Action_Roll(u8 *w);
void Player_SetAnimation(u8 *w, s32 a);
void Player_State_Crouch(void);

void Player_Action_Crouch(u8 *w) {
    s16 v = *(s16 *)(w + 0x34);

    if (v > 0x80 || v < -0x80) {
        Player_Action_Roll(w);
        return;
    }
    Player_SetAnimation(w, 3);
    *(u32 *)(w + 0x58) &= ~0x10;
    *(void **)(w + 0x68) = Player_State_Crouch;
    *(s16 *)(w + 0x34) = 0;
    *(s16 *)(w + 0x2e) = 0;
}
