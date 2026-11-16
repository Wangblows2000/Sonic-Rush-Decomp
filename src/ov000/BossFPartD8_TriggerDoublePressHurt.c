#include <nitro/types.h>

typedef struct UnkStruct_022f8904 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f8904;

extern UnkStruct_022f8904 *gBossF;

void BossFPartD8_Action_DoublePressHurt(void *p);

void BossFPartD8_TriggerDoublePressHurt(void) {
    BossFPartD8_Action_DoublePressHurt(gBossF->unk218);
}
