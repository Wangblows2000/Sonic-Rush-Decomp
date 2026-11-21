#include <nitro/types.h>

extern u8 data_ov000_02367d00;

void BossEffect_Create(void *p, s32 id, s32 a, s32 b, s32 c, s32 d);

void BossEffect_CreateSuction(s32 a, s32 b, s32 c, s32 d) {
    BossEffect_Create(&data_ov000_02367d00, 8, a, b, c, d);
}
