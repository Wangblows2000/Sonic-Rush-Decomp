#include <nitro/types.h>

typedef struct UnkStruct_022f88c0 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f88c0;

extern UnkStruct_022f88c0 *gBossF;

void BossFPartD8_Unk_ov000_022f2550(void *p);

void BossFPartD8_Unk_ov000_022f88c0(void) {
    BossFPartD8_Unk_ov000_022f2550(gBossF->unk218);
}
