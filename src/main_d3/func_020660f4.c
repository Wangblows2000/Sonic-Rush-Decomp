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

extern UnkColl_020660f4 *gMapCollisionMap;

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
        m = gMapCollisionMap;
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
        x = (x < gMapCollisionMap->left) ? gMapCollisionMap->left : ((x > gMapCollisionMap->right - 1) ? gMapCollisionMap->right - 1 : x);
        y = (y < gMapCollisionMap->top) ? gMapCollisionMap->top : ((y > gMapCollisionMap->bottom - 1) ? gMapCollisionMap->bottom - 1 : y);
    }

    tile = func_020665c8(x, y, flags & 1);
    id = tile & 0x3ff;
    xi = x & 7;
    if (tile & 0x400)
        xi = 7 - xi;
    h = gMapCollisionMap->height[id][xi] << 20 >> 24;
    if (h & 8)
        h = (s8)(h | ~0xf);
    if (h == -8)
        h = 8;
    if (flags & 0x80) {
        if (gMapCollisionMap->attr[id] & 1)
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
        a = gMapCollisionMap->angle[id];
        if (tile & 0x400)
            a = -(s8)a;
        if (tile & 0x800) {
            if (h != 0)
                a = -0x80 - (s8)a;
        }
        *angleOut = a;
    }
    if (attrOut != NULL && h != 0)
        *attrOut = gMapCollisionMap->attr[id];
    return h;
}
