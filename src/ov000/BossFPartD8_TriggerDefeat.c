#include <nitro/types.h>

typedef struct UnkStruct_022f87bc {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f87bc;

extern UnkStruct_022f87bc *gBossF;

void BossFPartD8_Action_Defeat(void *p);

void BossFPartD8_TriggerDefeat(void) {
    BossFPartD8_Action_Defeat(gBossF->unk218);
}
