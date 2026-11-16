#include <nitro/types.h>

void Player_LoadPhysicsParams(u8 *w);

#define ABS(x) ((x) < 0 ? -(x) : (x))

BOOL Player_CheckFallOffGround(u8 *w) {
    s32 v;

    if (!(*(u32 *)(w + 0x58) & 1)) {
        Player_LoadPhysicsParams(w);
        return TRUE;
    }
    if (w[0x3cc] != 0) {
        w[0x3cc]--;
    } else {
        if (!((w[1] + 0x40) & 0x80) && w[1] != 0xc0)
            return FALSE;
        v = *(s16 *)(w + 0x34);
        if (ABS(v) >= 0x1e0)
            return FALSE;
        Player_LoadPhysicsParams(w);
        return TRUE;
    }
    return FALSE;
}
