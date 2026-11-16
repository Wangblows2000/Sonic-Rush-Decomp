#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u32 type;
} UnkNode_022e6bb4;

typedef struct {
    u8 pad0[0x674];
    UnkNode_022e6bb4 nodes[17];
} UnkMap_022e6bb4;

typedef struct {
    s32 mode;
    u8 pad4[0x10];
    s32 cur;
    u8 pad18[8];
    s32 unk20;
    s32 unk24;
    u8 pad28[0xa4 - 0x28];
    UnkMap_022e6bb4 *map;
} UnkStruct_022e6bb4;

typedef struct {
    u16 cont;
} UnkPad_022e6bb4;

extern UnkPad_022e6bb4 data_022503e0;
extern s32 data_ov001_0231649c[][17][4];

#pragma thumb on
s32 func_ov001_022e6bb4(UnkStruct_022e6bb4 *p) {
    s32 dir;
    s32 next;
    UnkNode_022e6bb4 *nodes = p->map->nodes;
    s32 cur = p->cur;
    u32 t;
    s32 i;

    if (cur == 0x12) {
        return 0x12;
    }
    if (data_022503e0.cont & 0x20) {
        dir = 0;
    } else if (data_022503e0.cont & 0x40) {
        dir = 1;
    } else if (data_022503e0.cont & 0x10) {
        dir = 2;
    } else if (data_022503e0.cont & 0x80) {
        dir = 3;
    } else {
        return 0x12;
    }
    next = data_ov001_0231649c[p->mode][cur][dir];
    if (next == 0x12) {
        return 0x12;
    }
    if (nodes[next].type == 5) {
        next++;
    }
    t = nodes[next].type;
    if (t != 0 && t != 2 && t != 1) {
        return 0x12;
    }
    if (t == 2) {
        for (i = 0; i < 4; i++) {
            s32 v = data_ov001_0231649c[p->mode][next][i];
            if (v != 0x12 && v != cur) {
                p->unk20 = 1;
                p->unk24 = v;
                break;
            }
        }
    }
    return next;
}
#pragma thumb reset
