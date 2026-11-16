#include <nitro/types.h>

typedef struct UnkStruct_022f8904 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f8904;

extern UnkStruct_022f8904 *gBossF;

void func_ov000_022f2e58(void *p);

void func_ov000_022f8904(void) {
    func_ov000_022f2e58(gBossF->unk218);
}
