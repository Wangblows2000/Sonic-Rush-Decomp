#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 *tiles;
    u16 *layers[3];
    u16 w;
} UnkMap_020665c8;

extern UnkMap_020665c8 *gMapCollisionMap;

u16 MapCollision_GetTileAt(s32 x, s32 y, u8 layer)
{
    UnkMap_020665c8 *m = gMapCollisionMap;
    s32 tx = x >> 3;
    s32 ty = y >> 3;
    s32 bx = tx / 8;
    s32 by = ty / 8;
    return *(u16 *)((u8 *)m->tiles + (((tx - bx * 8) + (ty - by * 8) * 8) * 2 + m->layers[layer][m->w * by + bx] * 0x80));
}
