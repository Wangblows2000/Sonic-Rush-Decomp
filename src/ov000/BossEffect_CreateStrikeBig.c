#include <nitro/types.h>

typedef struct UnkStruct_023283e0 {
    u8 pad[0x13e];
    u8 unk13e;
    u8 unk13f;
    u8 unk140;
} UnkStruct_023283e0;

extern u8 data_ov000_02367d2c;

UnkStruct_023283e0 *BossEffect_Create(void *p, s32 id, s32 a, s32 b, s32 c, s32 d);

void BossEffect_CreateStrikeBig(s32 a, s32 b, s32 c, s32 d) {
    UnkStruct_023283e0 *s = BossEffect_Create(&data_ov000_02367d2c, 3, a, b, c, d);
    s->unk13e = 0x13;
    s->unk13f = 3;
    s->unk140 = 0;
}
