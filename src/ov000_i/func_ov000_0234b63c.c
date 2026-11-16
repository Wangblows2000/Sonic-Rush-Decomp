#include <nitro/types.h>

void Player_SetAnimation(u8 *w, s32 a);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void func_ov000_0234b63c(u8 *w) {
    s32 v = *(s16 *)(w + 0x34);
    u16 a = ABS(v);
    s32 anim;

    if (a < *(s16 *)(w + 0x378))
        anim = 0x3d;
    else if (a < *(s16 *)(w + 0x37a))
        anim = 0x3e;
    else if (a < *(s16 *)(w + 0x37c))
        anim = 0x3f;
    else if (a < *(s16 *)(w + 0x37e))
        anim = 0x40;
    else if (*(u32 *)(w + 0x32c) & 0x180) {
        if (a < *(s16 *)(w + 0x380))
            anim = 0x41;
        else
            anim = 0x42;
    } else
        anim = 0x40;
    Player_SetAnimation(w, anim);
    *(u16 *)(w + 0x5c) |= 4;
}
