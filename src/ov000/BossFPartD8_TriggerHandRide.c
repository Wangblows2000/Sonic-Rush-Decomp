#include <nitro/types.h>

typedef struct UnkStruct_022f88a4 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f88a4;

extern UnkStruct_022f88a4 *gBossF;

void BossFPartD8_Action_StartHandRide(void *p);

void BossFPartD8_TriggerHandRide(void) {
    BossFPartD8_Action_StartHandRide(gBossF->unk218);
}
