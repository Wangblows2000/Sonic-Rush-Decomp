#include <nitro/types.h>

typedef struct UnkStruct_022f8888 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f8888;

extern UnkStruct_022f8888 *gBossF;

void BossFPartD8_Action_ReturnHome(void *p);

void BossFPartD8_TriggerReturnHome(void) {
    BossFPartD8_Action_ReturnHome(gBossF->unk218);
}
