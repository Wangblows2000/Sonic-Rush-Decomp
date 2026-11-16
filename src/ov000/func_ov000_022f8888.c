#include <nitro/types.h>

typedef struct UnkStruct_022f8888 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f8888;

extern UnkStruct_022f8888 *gBossF;

void BossFPartD8_Action_Unk_ov000_022f1aac(void *p);

void func_ov000_022f8888(void) {
    BossFPartD8_Action_Unk_ov000_022f1aac(gBossF->unk218);
}
