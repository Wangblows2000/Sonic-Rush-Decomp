#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x434 - 0x6c];
    s32 unk434;
    u8 pad438[0x43c - 0x438];
    s32 unk43C;
    u16 unk440;
} UnkStruct_022f1930;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern u32 gRandSeed;
extern UnkStruct_02369ac0 gBossStatus;

void BossFPartD8_State_PartDCVolleyStart(UnkStruct_022f1930 *p);

void BossFPartD8_Action_StartPartDCVolley(UnkStruct_022f1930 *p) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    p->unk43C = ((u16)(gRandSeed >> 16) & 1) ? 1 : 0;
    p->unk440 = 0;
    p->unk434 = 0;
    gBossStatus.unk4 = 0x36;
    p->unk68 = BossFPartD8_State_PartDCVolleyStart;
}
