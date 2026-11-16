#include <nitro/types.h>

typedef struct UnkStruct_022f8948 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f8948;

extern UnkStruct_022f8948 *gBossF;

void BossFPartD8_Unk_ov000_022f3588(void *p);

void BossFPartD8_Unk_ov000_022f8948(void) {
    BossFPartD8_Unk_ov000_022f3588(gBossF->unk218);
}
