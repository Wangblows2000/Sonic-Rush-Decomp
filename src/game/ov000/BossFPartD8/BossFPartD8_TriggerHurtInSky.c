#include <nitro/types.h>

typedef struct UnkStruct_022f87d8 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f87d8;

extern UnkStruct_022f87d8 *gBossF;

void BossFPartD8_Action_HurtInSky(void *p);

void BossFPartD8_TriggerHurtInSky(void) {
    BossFPartD8_Action_HurtInSky(gBossF->unk218);
}
