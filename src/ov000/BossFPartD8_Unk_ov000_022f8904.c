#include <nitro/types.h>

typedef struct UnkStruct_022f8904 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f8904;

extern UnkStruct_022f8904 *gBossF;

void BossFPartD8_Unk_ov000_022f2e58(void *p);

void BossFPartD8_Unk_ov000_022f8904(void) {
    BossFPartD8_Unk_ov000_022f2e58(gBossF->unk218);
}
