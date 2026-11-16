#pragma opt_propagation off
#include <nitro/types.h>

typedef struct {
    u32 type;
    s16 x;
    u8 pad6[0x2a];
} UnkItem_022c8bc0;

typedef struct {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0x260 - 8];
    s16 bx;
    s16 by;
    u8 pad264[0x28c - 0x264];
    UnkItem_022c8bc0 items[6];
} UnkStruct_022c8bc0;

#pragma thumb on
static inline BOOL InRect_022c8bc0(u16 x, u16 y, u16 rx, u16 ry, s32 w, s32 h) {
    s32 ww = w; s32 hh = h;
    return x >= rx && x < rx + ww && y >= ry && y < ry + hh;
}

s32 func_ov001_022c8bc0(UnkStruct_022c8bc0 *p, u16 x, u16 y) {
    s32 v;
    s32 i;
    u16 ix;
    u16 iy;
    s16 yy;
    UnkItem_022c8bc0 *it;

    if (x >= 0x100) return 7;
    if (y >= 0xc0) return 7;
    yy = 0x18;
    i = 0;
    it = p->items;
    do {
        if (it->type != 7) {
            v = it->x - 0xb8;
            if (v < 0) v = 0;
            if (v >= 0x100) v = 0x100;
            ix = v;
            v = yy;
            if (v < 0) v = 0;
            if (v >= 0xc0) v = 0xc0;
            iy = v;
            if (InRect_022c8bc0(x, y, ix, iy, i == p->cur ? 0xc0 : 0xb8, 0x14)) {
                return i;
            }
        }
        it++;
        i++;
        yy += 0x18;
    } while (i < 6);
    v = p->bx;
    if (v < 0) v = 0;
    if (v >= 0x100) v = 0x100;
    ix = v;
    v = p->by;
    if (v < 0) v = 0;
    if (v >= 0xc0) v = 0xc0;
    iy = v;
    if (x >= ix && x < ix + 0x20 && y >= iy && y < iy + 0x20) {
        return 6;
    }
    return 7;
}
#pragma thumb reset
