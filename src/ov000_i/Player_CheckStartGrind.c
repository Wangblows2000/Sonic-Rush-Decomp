#include <nitro/types.h>

void Player_Action_Grind(u8 *w);

BOOL Player_CheckStartGrind(u8 *w) {
    if ((*(u32 *)(w + 0x58) & 1) && (*(u16 *)(w + 0x5e) & 8)) {
        if (*(s16 *)(w + 0x328) < 0x1a || *(s16 *)(w + 0x328) > 0x20)
            Player_Action_Grind(w);
        return TRUE;
    }
    return FALSE;
}
