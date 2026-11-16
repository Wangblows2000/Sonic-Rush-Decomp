#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 angle;
    u8 pad2[0xe];
    s32 x;
    s32 y;
    u8 pad18[0x44];
    u16 flags;
} UnkObj_02066bc0;

typedef struct {
    UnkObj_02066bc0 *obj;
    u32 unk4;
    u32 unk8;
    u16 flags;
    u16 pade;
    s8 (*map)[8];
    u8 *angles;
    u8 *attr;
    u8 pad1c[4];
    u16 w;
    u16 h;
} UnkEntry_02066bc0;

void func_02067c6c(UnkEntry_02066bc0 *e, s16 *ox, s16 *oy);

s8 func_02066bc0(UnkEntry_02066bc0 *e, s32 y, s32 x, u32 flags, u8 *outAng, u8 *outAttr) {
    s8 v;
    u16 col;
    u16 row;
    s16 ox;
    u16 idx;
    u32 sub;
    u8 a;
    s16 oy;

    ox = 0;
    oy = 0;
    func_02067c6c(e, &ox, &oy);
    if (x < (e->obj->x >> 8) + ox || x >= (e->obj->x >> 8) + ox + e->w * 8)
        return 0;
    if (y < (e->obj->y >> 8) + oy || y >= (e->obj->y >> 8) + oy + e->h * 8)
        return 0;
    col = (u32)(x - (e->obj->x >> 8) - ox) >> 3;
    row = (u32)(y - (e->obj->y >> 8) - oy) >> 3;
    if (e->obj != NULL) {
        if (e->obj->flags & 1)
            col = e->w - 1 - col;
        if (e->obj->flags & 2)
            row = e->h - 1 - row;
    }
    idx = row * e->w + col;
    sub = (x - (e->obj->x >> 8) - ox) & 7;
    if (e->obj != NULL && (e->obj->flags & 1))
        sub = 7 - sub;
    v = (s8)((u32)e->map[idx][sub] >> 4);
    if (v & 8)
        v = (s8)(v | ~0xf);
    if (v == -8)
        v = 8;
    if (e->attr != NULL && (flags & 0x80) && (*e->attr & 1))
        v = 0;
    if (e->obj != NULL && (e->obj->flags & 2) && v != 8 && v != 0) {
        if (v > 0)
            v -= 8;
        else
            v += 8;
    }
    if (outAng != NULL && v != 0) {
        if (e->angles != NULL && e->angles != (u8 *)-1)
            a = e->angles[idx];
        else
            a = 0;
        if (e->flags & 1)
            a += e->obj->angle;
        if (e->angles != NULL && e->obj != NULL) {
            if (!(e->obj->flags & 1))
                a = -(s8)a;
            if (e->obj->flags & 2)
                a = -0x80 - (s8)a;
        }
        *outAng = a;
    }
    if (outAttr == NULL || v == 0)
        return v;
    if (e->attr != NULL)
        *outAttr = *e->attr;
    return v;
}
