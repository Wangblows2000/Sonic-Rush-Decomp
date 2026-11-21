#pragma opt_propagation off
#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x4c - 4];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s8 unk5C;
    s8 unk5D;
    s8 unk5E;
    s8 unk5F;
    s8 unk60;
    u8 pad61;
    u16 unk62;
} UnkPlayer_0233c00c;


typedef struct {
    u32 flags;
    u8 pad4[0x4c - 4];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
} UnkGlobal_0233c00c;

typedef struct {
    u8 pad0[0x24];
    s16 unk24;
} UnkObj_0233c00c;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
} UnkCfg_0233c00c;

extern UnkPlayer_0233c00c gCamera[];
extern UnkPlayer_0233c00c data_ov000_0236ab3c;
extern UnkCfg_0233c00c gGameState;
extern UnkObj_0233c00c *gDisplayTable[2];

void Camera_ResetOnRespawn(void) {
    u8 cfg;

    if (gCamera[2].flags & 1)
        return;
    gCamera[2].flags &= 0xfff8fefb;
    gCamera[0].flags &= ~0xf000;
    gCamera[0].unk4C = gCamera[2].unk4C;
    gCamera[0].unk50 = gCamera[2].unk50;
    gCamera[0].unk54 = gCamera[2].unk54;
    gCamera[0].unk58 = gCamera[2].unk58;
    gCamera[0].unk5D = gCamera[0].unk5E = gCamera[0].unk5F = 0;
    gCamera[0].unk60 = 0;
    gCamera[0].unk5C = gCamera[0].unk5D;
    cfg = gGameState.unk1C;
    if (cfg == 2) {
        gCamera[0].flags &= ~0x1000000;
        gCamera[0].unk62 = 0xffff;
        UnkPlayer_0233c00c *q = &data_ov000_0236ab3c;
        q->flags &= ~0x1000000;
        q->unk62 = 0xffff;
    }
    if (gGameState.unk1C == 3) {
        gDisplayTable[0]->unk24 = 0;
        gDisplayTable[1]->unk24 = 0;
    }
}
