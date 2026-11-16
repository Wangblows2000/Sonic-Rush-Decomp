#include <nitro/types.h>

typedef struct UnkStruct_022f8810_2 {
    u8 pad[0x458];
    s32 unk458;
} UnkStruct_022f8810_2;

typedef struct UnkStruct_022f8964 {
    u8 pad[0x218];
    UnkStruct_022f8810_2 *unk218;
} UnkStruct_022f8964;

extern UnkStruct_022f8964 *gBossF;

void func_ov000_022f3e00(UnkStruct_022f8810_2 *p);

void func_ov000_022f8964(s32 a) {
    gBossF->unk218->unk458 = a;
    func_ov000_022f3e00(gBossF->unk218);
}
