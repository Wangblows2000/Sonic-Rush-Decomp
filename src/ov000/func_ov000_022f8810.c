#include <nitro/types.h>

typedef struct UnkStruct_022f8810_2 {
    u8 pad[0x458];
    s32 unk458;
} UnkStruct_022f8810_2;

typedef struct UnkStruct_022f8810 {
    u8 pad[0x218];
    UnkStruct_022f8810_2 *unk218;
} UnkStruct_022f8810;

extern UnkStruct_022f8810 *gBossF;

void func_ov000_022f0f44(UnkStruct_022f8810_2 *p);

void func_ov000_022f8810(s32 a) {
    gBossF->unk218->unk458 = a;
    func_ov000_022f0f44(gBossF->unk218);
}
