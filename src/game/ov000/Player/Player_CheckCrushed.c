#include <nitro/types.h>

void Player_Action_Die(u8 *w);

void Player_CheckCrushed(u8 *w) {
    if ((*(u32 *)(w + 0x32c) & 0x800) && *(void **)(w + 0x84) == NULL && (*(u32 *)(w + 0x58) & 4) && (*(u32 *)(w + 0x58) & 8))
        Player_Action_Die(w);
    if (*(void **)(w + 0x84) != NULL && !(*(u32 *)(w + 0x32c) & 0x1000)) {
        if (((*(u32 *)(w + 0x58) & 1) && (*(u32 *)(w + 0x58) & 2)) || ((*(u32 *)(w + 0x58) & 4) && (*(u32 *)(w + 0x58) & 8))) {
            Player_Action_Die(w);
            return;
        }
        *(s16 *)(w + 0x3bc) = 0;
        return;
    }
    *(s16 *)(w + 0x3bc) = 0;
}
