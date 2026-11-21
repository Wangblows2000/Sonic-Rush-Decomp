#include <nitro/types.h>

s32 Player_InitState();
void Player_Action_AttackBounce(u8 *w);

void Player_Action_AttackRecoil(u8 *w) {
    Player_InitState(w);
    *(u32 *)(w + 0x58) |= 0x10;
    if (*(s16 *)(w + 0x28) > 0)
        *(s16 *)(w + 0x2e) = -0x180;
    else
        *(s16 *)(w + 0x2e) = 0x180;
    *(s16 *)(w + 0x352) = 0x18;
    Player_Action_AttackBounce(w);
}
