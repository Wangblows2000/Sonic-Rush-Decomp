#include <nitro/types.h>

typedef struct UnkStruct_022f8810_2 {
    u8 pad[0x458];
    s32 unk458;
} UnkStruct_022f8810_2;

typedef struct UnkStruct_022f8838 {
    u8 pad[0x218];
    UnkStruct_022f8810_2 *unk218;
} UnkStruct_022f8838;

extern UnkStruct_022f8838 *gBossF;

void BossFPartD8_Action_StartPartDEAttack(UnkStruct_022f8810_2 *p);

void BossFPartD8_TriggerPartDEAttack(s32 a) {
    gBossF->unk218->unk458 = a;
    BossFPartD8_Action_StartPartDEAttack(gBossF->unk218);
}
