#include <nitro/types.h>

typedef struct UnkStruct_022f87f4 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f87f4;

extern UnkStruct_022f87f4 *gBossF;

void func_ov000_022efbd8(void *p);

void func_ov000_022f87f4(void) {
    func_ov000_022efbd8(gBossF->unk218);
}
