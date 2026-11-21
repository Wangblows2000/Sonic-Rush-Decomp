#include <nitro/types.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Player_SetAnimSpeedFromVelocity(u8 *w, s32 v) {
    s32 t = (v >> 5) + (v >> 6);
    s16 s = ABS(t);
    u8 *o;

    if (s <= 0x10)
        s = 0x10;
    if (s >= 0x80)
        s = 0x80;
    *(s16 *)(w + 0xbe) = s;
    *(s16 *)(w + 0x106) = s;
    o = *(u8 **)(w + 0x124);
    if (o != NULL)
        *(s32 *)(o + 0x114) = (s << 12) >> 4;
}
