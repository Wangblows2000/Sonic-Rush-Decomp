#include <nitro/types.h>

typedef struct UnkStruct_022f87d8 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f87d8;

extern UnkStruct_022f87d8 *gBossF;

void func_ov000_022ef9c8(void *p);

void func_ov000_022f87d8(void) {
    func_ov000_022ef9c8(gBossF->unk218);
}
