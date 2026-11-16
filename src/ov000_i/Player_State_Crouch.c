#include <nitro/types.h>

void Player_ApplySlopeForce(u8 *w, s32 a);
BOOL Player_CheckFallOffGround(u8 *w);
void Player_Action_Fall(u8 *w);
void Player_Action_Roll(u8 *w);
void Player_Action_SpinDash(u8 *w);

void Player_State_Crouch(u8 *w) {
    s16 v;

    if (((w[1] + 0x30) & 0xff) >= 0x60)
        Player_ApplySlopeForce(w, 0);
    if (Player_CheckFallOffGround(w)) {
        Player_Action_Fall(w);
        return;
    }
    v = *(s16 *)(w + 0x34);
    if (v > 0x80 || v < -0x80) {
        Player_Action_Roll(w);
        return;
    }
    if (v == 0 && *(s16 *)(w + 0x2e) == 0 && (*(u16 *)(w + 0x422) & 3)) {
        Player_Action_SpinDash(w);
        return;
    }
    if (!(*(u16 *)(w + 0x420) & 0x80))
        (*(void (**)(u8 *))(w + 0x334))(w);
}
