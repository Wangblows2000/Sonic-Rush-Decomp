#include <nitro/types.h>

typedef struct UnkStruct_022f88c0 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f88c0;

extern UnkStruct_022f88c0 *gBossF;

void BossFPartD8_Action_PartDAThrowHurt(void *p);

void BossFPartD8_TriggerPartDAThrowHurt(void) {
    BossFPartD8_Action_PartDAThrowHurt(gBossF->unk218);
}
