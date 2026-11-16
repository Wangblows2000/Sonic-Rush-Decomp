#include <nitro/types.h>

typedef struct UnkStruct_022f88a4 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f88a4;

extern UnkStruct_022f88a4 *gBossF;

void BossFPartD8_Unk_ov000_022f2444(void *p);

void BossFPartD8_Unk_ov000_022f88a4(void) {
    BossFPartD8_Unk_ov000_022f2444(gBossF->unk218);
}
