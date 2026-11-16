#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x248 - 0x20C];
    s32 unk248;
} UnkStruct_022f8a48;

UnkStruct_022f8a48 *BossEffect_CreateLaserFinB(s32 a, s32 x, s32 y, s32 z);

void BossF_Unk_ov000_022f8a48(void) {
    UnkStruct_022f8a48 *o;
    s32 x;
    s32 y;
    s32 z;

    x = ((Rand() & 0x1ff) + (Rand() & 0xff)) << 8;
    y = ((Rand() & 0x1f) << 10) + 0x30000;
    z = ((Rand() & 0xf) << 10) + 0x3000;
    o = BossEffect_CreateLaserFinB(1, x, y, z);
    o->base.unk30 = -0x1000;
    o->unk248 = 0x1599;
}
