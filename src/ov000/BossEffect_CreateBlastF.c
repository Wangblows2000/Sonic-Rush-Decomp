#include <nitro/types.h>

extern u8 data_ov000_02367dcc;

void BossEffect_Create(void *p, s32 id, s32 a, s32 b, s32 c, s32 d);

void BossEffect_CreateBlastF(s32 a, s32 b, s32 c, s32 d) {
    BossEffect_Create(&data_ov000_02367dcc, 7, a, b, c, d);
}
