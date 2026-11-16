#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022f89a8_plr;

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
} UnkStruct_022c4534;

extern UnkStruct_022f89a8_plr *gPlayers;
extern UnkStruct_022c4534 gStageMapCollision;

BOOL func_ov000_022f89a8(void) {
    s32 v;

    v = (gPlayers->unk10 >> 8) + 0xc;
    if (v < gStageMapCollision.unk24)
        gStageMapCollision.unk24 = v;
    if (gStageMapCollision.unk24 > 0x340)
        return FALSE;
    gStageMapCollision.unk24 = 0x340;
    return TRUE;
}
