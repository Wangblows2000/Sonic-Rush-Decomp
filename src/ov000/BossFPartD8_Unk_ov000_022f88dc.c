#include <nitro/types.h>

typedef struct UnkStruct_022f8810_2 {
    u8 pad[0x458];
    s32 unk458;
} UnkStruct_022f8810_2;

typedef struct UnkStruct_022f88dc {
    u8 pad[0x218];
    UnkStruct_022f8810_2 *unk218;
} UnkStruct_022f88dc;

extern UnkStruct_022f88dc *gBossF;

void BossFPartD8_Unk_ov000_022f2d50(UnkStruct_022f8810_2 *p);

void BossFPartD8_Unk_ov000_022f88dc(s32 a) {
    gBossF->unk218->unk458 = a;
    BossFPartD8_Unk_ov000_022f2d50(gBossF->unk218);
}
