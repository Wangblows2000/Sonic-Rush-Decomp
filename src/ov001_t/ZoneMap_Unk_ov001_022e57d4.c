#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 type;
} UnkNode_022e57d4;

typedef struct {
    u8 pad0[0x674];
    UnkNode_022e57d4 nodes[17];
} UnkMap_022e57d4;

typedef struct {
    u8 pad0[8];
    s32 level;
    u8 padc[0xa4 - 0xc];
    UnkMap_022e57d4 *map;
} UnkStruct_022e57d4;

void ZoneMap_Unk_ov001_022e57a4(UnkStruct_022e57d4 *p, s32 a, s32 b);

#pragma thumb on
void ZoneMap_Unk_ov001_022e57d4(UnkStruct_022e57d4 *p) {
    s32 lv = p->level;
    UnkNode_022e57d4 *n = p->map->nodes;

    n[0].type = 0;
    if (lv >= 3) {
        n[1].type = 0;
        ZoneMap_Unk_ov001_022e57a4(p, 0xb, 0);
    }
    if (lv >= 0xb) {
        n[2].type = 0;
        ZoneMap_Unk_ov001_022e57a4(p, 0xd, 0);
    }
    if (lv >= 0xd) {
        n[3].type = 0;
        ZoneMap_Unk_ov001_022e57a4(p, 0xe, 0);
    }
    if (lv >= 0x11) {
        n[6].type = 0;
        ZoneMap_Unk_ov001_022e57a4(p, 0x10, 0);
    }
    if (lv >= 0x15) {
        n[8].type = 0;
        ZoneMap_Unk_ov001_022e57a4(p, 0x11, 0);
    }
    if (lv >= 0x19) {
        n[12].type = 0;
        ZoneMap_Unk_ov001_022e57a4(p, 0x13, 0);
    }
    if (lv >= 0x1d) {
        n[13].type = 0;
        ZoneMap_Unk_ov001_022e57a4(p, 0x14, 0);
    }
    if (lv >= 0x20) {
        n[14].type = 0;
        ZoneMap_Unk_ov001_022e57a4(p, 0x15, 0);
    }
    if (lv >= 6) {
        n[16].type = 0;
        ZoneMap_Unk_ov001_022e57a4(p, 0xc, 0);
    }
    if (lv >= 0x14) {
        n[5].type = 1;
        n[4].type = 5;
    } else if (lv >= 0x13) {
        n[5].type = 0;
        n[4].type = 5;
    } else if (lv >= 0x10) {
        n[5].type = 1;
        n[4].type = 5;
    } else if (lv >= 0xf) {
        n[4].type = 0;
    }
    if (lv >= 0xf) {
        ZoneMap_Unk_ov001_022e57a4(p, 0xf, 0);
    }
    if (lv >= 0x1c) {
        n[11].type = 1;
        n[10].type = 5;
    } else if (lv >= 0x1b) {
        n[11].type = 0;
        n[10].type = 5;
    } else if (lv >= 0x18) {
        n[11].type = 1;
        n[10].type = 5;
    } else if (lv >= 0x17) {
        n[10].type = 0;
    }
    if (lv >= 0x17) {
        ZoneMap_Unk_ov001_022e57a4(p, 0x12, 0);
    }
}
#pragma thumb reset
