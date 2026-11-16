#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkObj_02067500;

typedef struct {
    UnkObj_02067500 *obj;
} UnkColl_02067500;

void ObjCollisionMap_GetOffset(UnkColl_02067500 *s, s16 *x, s16 *y);
s8 ObjCollisionMap_GetTileWidth(UnkColl_02067500 *c, s32 x, s32 y, u32 flags, u8 *outA, u8 *outB);
s8 ObjCollisionMap_GetTileHeight(UnkColl_02067500 *c, s32 x, s32 y, u32 flags, u8 *outA, u8 *outB);

s32 ObjCollisionMap_ProbeEntry(UnkColl_02067500 *c, s32 x, s32 y, u32 flags, u8 mode, u8 *outA, u8 *outB) {
    u8 sa;
    u8 sb;
    s32 dir = 8;
    s32 v;
    s8 sub;
    s16 ox = 0;
    s16 oy = 0;

    ObjCollisionMap_GetOffset(c, &ox, &oy);
    if (outA != NULL)
        sa = *outA;
    if (outB != NULL)
        sb = *outB;
    if (mode & 1)
        dir = -8;
    if (mode & 2) {
        v = ObjCollisionMap_GetTileHeight(c, y, x, flags, outA, outB);
        sub = (y - ((c->obj->y >> 8) + oy)) & 7;
    } else {
        v = ObjCollisionMap_GetTileWidth(c, x, y, flags, outA, outB);
        sub = (x - ((c->obj->x >> 8) + ox)) & 7;
    }
    if (v == 0) {
        if (outA != NULL)
            *outA = sa;
        if (outB != NULL)
            *outB = sb;
        return dir > 0 ? 8 - sub : sub + 1;
    }
    if (v == 8)
        return dir > 0 ? -(sub + 1) : sub - 8;
    if (v > 0)
        return dir > 0 ? v - (sub + 1) : 8 - sub;
    return dir > 0 ? -(sub + 1) : v + sub;
}
