#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x368 - 0x1c];
    s32 unk368;
} UnkStruct_022f5c78_t;

typedef struct {
    u8 pad0[0x218];
    UnkStruct_022f5c78_t *unk218;
} UnkStruct_022f5c78_g;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x30 - 0x1c];
    s16 unk30;
    u8 pad32[0x68 - 0x32];
    void *unk68;
} UnkStruct_022f5c78;

extern UnkStruct_022f5c78_g *gBossF;

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void BossFPartDE_State_ChainRise(UnkStruct_022f5c78 *p);

void BossFPartDE_StartChainRise(UnkStruct_022f5c78 *p) {
    UnkStruct_022f5c78_t *t = gBossF->unk218;

    p->unk10 = t->unk10;
    p->unk14 = -t->unk368 >> 4;
    p->unk18 = t->unk18 - 0x3200;
    p->unk30 = -0x1000;
    Sound_PlaySeqArc(0, 0, 0xe4);
    p->unk68 = BossFPartDE_State_ChainRise;
}
