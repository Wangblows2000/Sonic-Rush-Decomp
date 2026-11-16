#pragma opt_propagation off
#include <nitro/types.h>

typedef struct {
    s8 (*height)[8];
    u8 *angle;
    u8 pad8[0xc];
    u8 *attr;
    u8 pad18[0x4];
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} UnkColl_02066360;

extern UnkColl_02066360 *data_022c4dc8;

s32 func_020665c8(s32 x, s32 y, u8 layer);

s32 func_02066360(s32 x, s32 y, u32 flags, u8 *angleOut, u8 *attrOut) {
    u16 tile;
    u16 id;
    s32 yi;
    s32 h;
    u8 a;
    s32 tx;
    s32 right;
    UnkColl_02066360 *m;

    if (flags & 0x40) {
        m = data_022c4dc8;
        right = m->right - 1;
        tx = x & ~7;
        if (tx > right || x < m->left - 7)
            return 8;
        if (tx + 8 > right) {
            h = (s8)(right & 7);
            if (h == 0)
                h = 8;
            return h;
        }
        if (tx < m->left) {
            s8 t;
            if (m->left & 7)
                t = 0x10 - (m->left & 7);
            else
                t = 8;
            t |= ~0xf;
            if (t == -8)
                t = 8;
            return t;
        }
        if (y > m->bottom - 1 || y < m->top)
            return 8;
    } else {
        x = (x < data_022c4dc8->left) ? data_022c4dc8->left : ((x > data_022c4dc8->right - 1) ? data_022c4dc8->right - 1 : x);
        y = (y < data_022c4dc8->top) ? data_022c4dc8->top : ((y > data_022c4dc8->bottom - 1) ? data_022c4dc8->bottom - 1 : y);
    }

    tile = func_020665c8(x, y, flags & 1);
    id = tile & 0x3ff;
    yi = y & 7;
    if (tile & 0x800)
        yi = 7 - yi;
    h = (s8)(data_022c4dc8->height[id][yi] & 0xf);
    if (h & 8)
        h = (s8)(h | ~0xf);
    if (h == -8)
        h = 8;
    if (flags & 0x80) {
        if (data_022c4dc8->attr[id] & 1)
            h = 0;
    }
    if (tile & 0x400) {
        if (h != 8 && h != 0) {
            if (h > 0)
                h = (s8)(h - 8);
            else
                h = (s8)(h + 8);
        }
    }
    if (angleOut != NULL && h != 0) {
        a = data_022c4dc8->angle[id];
        if (tile & 0x800)
            a = -0x80 - (s8)a;
        if (tile & 0x400) {
            if (h != 0)
                a = -(s8)a;
        }
        *angleOut = a;
    }
    if (attrOut != NULL && h != 0)
        *attrOut = data_022c4dc8->attr[id];
    return h;
}
