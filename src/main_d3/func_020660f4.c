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
} UnkColl_020660f4;

extern UnkColl_020660f4 *data_022c4dc8;

s32 func_020665c8(s32 x, s32 y, u8 layer);

s32 func_020660f4(s32 y, s32 x, u32 flags, u8 *angleOut, u8 *attrOut) {
    u16 tile;
    u16 id;
    s32 xi;
    s32 h;
    u8 a;
    s32 ty;
    s32 bottom;
    UnkColl_020660f4 *m;

    if (flags & 0x40) {
        m = data_022c4dc8;
        bottom = m->bottom - 1;
        ty = y & ~7;
        if (ty > bottom || y < m->top - 7)
            return 8;
        if (ty + 8 > bottom) {
            h = (s8)(bottom & 7);
            if (h == 0)
                h = 8;
            return h;
        }
        if (ty < m->top) {
            s8 t;
            if (m->top & 7)
                t = 0x10 - (m->top & 7);
            else
                t = 8;
            t |= ~0xf;
            if (t == -8)
                t = 8;
            return t;
        }
        if (x > m->right - 1 || x < m->left)
            return 8;
    } else {
        x = (x < data_022c4dc8->left) ? data_022c4dc8->left : ((x > data_022c4dc8->right - 1) ? data_022c4dc8->right - 1 : x);
        y = (y < data_022c4dc8->top) ? data_022c4dc8->top : ((y > data_022c4dc8->bottom - 1) ? data_022c4dc8->bottom - 1 : y);
    }

    tile = func_020665c8(x, y, flags & 1);
    id = tile & 0x3ff;
    xi = x & 7;
    if (tile & 0x400)
        xi = 7 - xi;
    h = data_022c4dc8->height[id][xi] << 20 >> 24;
    if (h & 8)
        h = (s8)(h | ~0xf);
    if (h == -8)
        h = 8;
    if (flags & 0x80) {
        if (data_022c4dc8->attr[id] & 1)
            h = 0;
    }
    if (tile & 0x800) {
        if (h != 8 && h != 0) {
            if (h > 0)
                h = (s8)(h - 8);
            else
                h = (s8)(h + 8);
        }
    }
    if (angleOut != NULL && h != 0) {
        a = data_022c4dc8->angle[id];
        if (tile & 0x400)
            a = -(s8)a;
        if (tile & 0x800) {
            if (h != 0)
                a = -0x80 - (s8)a;
        }
        *angleOut = a;
    }
    if (attrOut != NULL && h != 0)
        *attrOut = data_022c4dc8->attr[id];
    return h;
}
