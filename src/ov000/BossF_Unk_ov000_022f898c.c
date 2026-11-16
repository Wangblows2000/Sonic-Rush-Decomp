#include <nitro/types.h>

typedef struct UnkStruct_022f898c {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f898c;

extern UnkStruct_022f898c *gBossF;

void BossF_Unk_ov000_022f3ed4(void *p);

void BossF_Unk_ov000_022f898c(void) {
    BossF_Unk_ov000_022f3ed4(gBossF->unk218);
}
