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

extern UnkColl_02066360 *gMapCollisionMap;

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
        m = gMapCollisionMap;
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
        x = (x < gMapCollisionMap->left) ? gMapCollisionMap->left : ((x > gMapCollisionMap->right - 1) ? gMapCollisionMap->right - 1 : x);
        y = (y < gMapCollisionMap->top) ? gMapCollisionMap->top : ((y > gMapCollisionMap->bottom - 1) ? gMapCollisionMap->bottom - 1 : y);
    }

    tile = func_020665c8(x, y, flags & 1);
    id = tile & 0x3ff;
    yi = y & 7;
    if (tile & 0x800)
        yi = 7 - yi;
    h = (s8)(gMapCollisionMap->height[id][yi] & 0xf);
    if (h & 8)
        h = (s8)(h | ~0xf);
    if (h == -8)
        h = 8;
    if (flags & 0x80) {
        if (gMapCollisionMap->attr[id] & 1)
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
        a = gMapCollisionMap->angle[id];
        if (tile & 0x800)
            a = -0x80 - (s8)a;
        if (tile & 0x400) {
            if (h != 0)
                a = -(s8)a;
        }
        *angleOut = a;
    }
    if (attrOut != NULL && h != 0)
        *attrOut = gMapCollisionMap->attr[id];
    return h;
}
