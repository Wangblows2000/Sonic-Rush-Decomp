#include <nitro/types.h>

typedef struct UnkStruct_022f87f4 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f87f4;

extern UnkStruct_022f87f4 *gBossF;

void BossFPartD8_Action_Hurt(void *p);

void BossFPartD8_TriggerHurt(void) {
    BossFPartD8_Action_Hurt(gBossF->unk218);
}
