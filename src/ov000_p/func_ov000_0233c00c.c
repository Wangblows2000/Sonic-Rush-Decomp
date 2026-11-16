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

extern UnkPlayer_0233c00c data_ov000_0236aad8[];
extern UnkPlayer_0233c00c data_ov000_0236ab3c;
extern UnkCfg_0233c00c data_022c4560;
extern UnkObj_0233c00c *data_02072c78[2];

void func_ov000_0233c00c(void) {
    u8 cfg;

    if (data_ov000_0236aad8[2].flags & 1)
        return;
    data_ov000_0236aad8[2].flags &= 0xfff8fefb;
    data_ov000_0236aad8[0].flags &= ~0xf000;
    data_ov000_0236aad8[0].unk4C = data_ov000_0236aad8[2].unk4C;
    data_ov000_0236aad8[0].unk50 = data_ov000_0236aad8[2].unk50;
    data_ov000_0236aad8[0].unk54 = data_ov000_0236aad8[2].unk54;
    data_ov000_0236aad8[0].unk58 = data_ov000_0236aad8[2].unk58;
    data_ov000_0236aad8[0].unk5D = data_ov000_0236aad8[0].unk5E = data_ov000_0236aad8[0].unk5F = 0;
    data_ov000_0236aad8[0].unk60 = 0;
    data_ov000_0236aad8[0].unk5C = data_ov000_0236aad8[0].unk5D;
    cfg = data_022c4560.unk1C;
    if (cfg == 2) {
        data_ov000_0236aad8[0].flags &= ~0x1000000;
        data_ov000_0236aad8[0].unk62 = 0xffff;
        UnkPlayer_0233c00c *q = &data_ov000_0236ab3c;
        q->flags &= ~0x1000000;
        q->unk62 = 0xffff;
    }
    if (data_022c4560.unk1C == 3) {
        data_02072c78[0]->unk24 = 0;
        data_02072c78[1]->unk24 = 0;
    }
}
