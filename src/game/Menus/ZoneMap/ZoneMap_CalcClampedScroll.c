#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 w;
    s32 h;
} UnkStruct_022e5164;

#pragma thumb on
void ZoneMap_CalcClampedScroll(UnkStruct_022e5164 *p, s32 x, s32 y, s32 *ox, s32 *oy) {
    s32 mx;
    s32 my;

    mx = p->w - 0x108;
    my = p->h - 0xc0;
    x -= 0x80;
    y -= 0x60;
    if (x < 8) x = 8;
    if (x > mx) x = mx;
    if (y < 0) y = 0;
    if (y > my) y = my;
    *ox = x;
    *oy = y;
}
#pragma thumb reset
