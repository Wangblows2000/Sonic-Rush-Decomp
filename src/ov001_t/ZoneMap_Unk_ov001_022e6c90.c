#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e6c90;

typedef struct {
    u8 pad0[0xc];
    u32 type;
} UnkNode_022e6c90;

typedef struct {
    u8 pad0[4];
    s32 scrollX;
    s32 scrollY;
    u8 padc[0x674 - 0xc];
    UnkNode_022e6c90 nodes[17];
} UnkMap_022e6c90;

typedef struct {
    s32 mode;
    u8 pad4[0x1c];
    s32 count;
    s32 stack[8];
    u8 pad44[0xa4 - 0x44];
    UnkMap_022e6c90 *map;
} UnkStruct_022e6c90;

extern UnkTouch_022e6c90 gTouchState;
extern s32 data_ov001_0231649c[][17][4];

BOOL ZoneMap_Unk_ov001_022e690c(UnkStruct_022e6c90 *p, s32 node, s32 x, s32 y);

#pragma thumb on
s32 ZoneMap_Unk_ov001_022e6c90(UnkStruct_022e6c90 *p, s32 node, s32 prev) {
    s32 x, y;
    s32 i;
    s32 next;
    s32 r;
    UnkNode_022e6c90 *nd;

    if (p->count > 0) {
        p->count--;
        return p->stack[p->count];
    }
    if (!(gTouchState.flags & 0x40)) {
        return 0x12;
    }
    x = gTouchState.x;
    y = gTouchState.y;
    if (x < 0 || x >= 0x100 || y < 0 || y >= 0xc0) {
        return 0x12;
    }
    x += p->map->scrollX;
    y += p->map->scrollY;
    if (p->map->nodes[node].type <= 1 && ZoneMap_Unk_ov001_022e690c(p, node, x, y)) {
        return node;
    }
    for (i = 0; i < 4; i++) {
        next = data_ov001_0231649c[p->mode][node][i];
        if (next == 0x12) continue;
        nd = &p->map->nodes[next];
        if (nd->type == 5) {
            next++;
            nd = &p->map->nodes[next];
        }
        if (next == prev) continue;
        if (nd->type == 3 || nd->type == 4) continue;
        if (ZoneMap_Unk_ov001_022e690c(p, next, x, y)) {
            if (nd->type > 1) {
                next = 0x12;
            }
            return next;
        }
        r = ZoneMap_Unk_ov001_022e6c90(p, next, node);
        if (r == 0x12) continue;
        if (p->count < 8) {
            p->stack[p->count] = r;
            p->count++;
            return next;
        }
        p->count = 0;
        return 0x12;
    }
    return 0x12;
}
#pragma thumb reset
