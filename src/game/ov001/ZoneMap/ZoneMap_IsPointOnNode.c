#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 unk8;
    u32 type;
} UnkNode_022e690c;

typedef struct {
    u8 pad0[0x674];
    UnkNode_022e690c nodes[17];
} UnkMap_022e690c;

typedef struct {
    u8 pad0[0xa4];
    UnkMap_022e690c *map;
} UnkStruct_022e690c;

typedef struct {
    s8 x0;
    s8 y0;
    s8 x1;
    s8 y1;
} UnkRect_022e690c;

extern UnkRect_022e690c data_ov001_02316358[];

#pragma thumb on
BOOL ZoneMap_IsPointOnNode(UnkStruct_022e690c *p, s32 node, s32 x, s32 y) {
    UnkNode_022e690c *nd = &p->map->nodes[node];
    s32 x0, y0, x1, y1;

    switch (nd->type) {
    case 0:
        x0 = nd->x + data_ov001_02316358[node].x0;
        y0 = nd->y + data_ov001_02316358[node].y0;
        x1 = nd->x + data_ov001_02316358[node].x1;
        y1 = nd->y + data_ov001_02316358[node].y1;
        break;
    case 1:
    case 3:
        x0 = nd->x - 12;
        y0 = nd->y - 8;
        x1 = nd->x + 12;
        y1 = nd->y + 8;
        break;
    default:
        return FALSE;
    }
    if (x >= x0 && x < x1 && y >= y0 && y < y1) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
