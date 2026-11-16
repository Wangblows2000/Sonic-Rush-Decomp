#include <nitro/types.h>

typedef struct UnkStruct_022f898c {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f898c;

extern UnkStruct_022f898c *gBossF;

void BossF_PartD8StartWaitNextAttack(void *p);

void BossF_StartNextAttack(void) {
    BossF_PartD8StartWaitNextAttack(gBossF->unk218);
}
