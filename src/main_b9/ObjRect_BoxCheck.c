#include <nitro/types.h>

typedef struct UnkCol_0206d7c8 {
    s32 x;
    s32 y;
    s16 z;
    u16 flags;
    s16 l;
    s16 t;
    s16 r;
    s16 b;
} UnkCol_0206d7c8;

BOOL ObjRect_BoxCheck(UnkCol_0206d7c8 *a, s32 x, s32 y, s32 ox, s16 oy, u16 w, u16 h) {
    s32 py;
    s32 ax;
    u16 ah;
    s32 px;
    s32 ay;
    u16 aw;

    if ((a->flags & 4) && !(a->flags & 0x1000)) {
        if (a->flags & 1)
            ax = a->x - a->r;
        else
            ax = a->x + a->l;
        if (a->flags & 2)
            ay = a->y - a->b;
        else
            ay = a->y + a->t;
        py = y + oy;
        aw = a->r - a->l;
        ah = a->b - a->t;
        px = x + ox;
        if (((ax <= px && ax + aw >= px) || (ax >= px && px + w >= ax))
            && ((ay <= py && ay + ah >= py) || (ay >= py && py + h >= ay)))
            return TRUE;
    }
    return FALSE;
}
