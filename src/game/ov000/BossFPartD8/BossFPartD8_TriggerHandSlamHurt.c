#include <nitro/types.h>

typedef struct UnkStruct_022f8948 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f8948;

extern UnkStruct_022f8948 *gBossF;

void BossFPartD8_Action_HandSlamHurt(void *p);

void BossFPartD8_TriggerHandSlamHurt(void) {
    BossFPartD8_Action_HandSlamHurt(gBossF->unk218);
}
