#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x438 - 0x6c];
    u16 unk438;
    u16 unk43A;
} UnkStruct_022f300c;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    u16 unk8;
} UnkStruct_02369ac0;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern u16 data_ov000_023642b4[][8];

void BossFPartD8_State_DoublePressVulnerable(UnkStruct_022f300c *p);

void BossFPartD8_State_DoublePressStartVulnerable(UnkStruct_022f300c *p) {
    BOOL b;
    s32 idx;

    b = gGameState.unk10 == 2 || gGameState.unk10 == 3;
    if (b)
        idx = 1;
    else
        idx = gGameState.unk14;
    p->unk438 = data_ov000_023642b4[idx][gBossStatus.unk8 - 1];
    p->unk43A = 0;
    p->unk68 = BossFPartD8_State_DoublePressVulnerable;
}
