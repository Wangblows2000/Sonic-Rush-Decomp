#include <nitro/types.h>

typedef struct UnkStruct_022f87bc {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f87bc;

extern UnkStruct_022f87bc *gBossF;

void BossFPartD8_Unk_ov000_022ef7b0(void *p);

void BossFPartD8_Unk_ov000_022f87bc(void) {
    BossFPartD8_Unk_ov000_022ef7b0(gBossF->unk218);
}
