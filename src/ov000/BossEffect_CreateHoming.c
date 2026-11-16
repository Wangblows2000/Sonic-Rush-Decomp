#include <nitro/types.h>

extern u8 data_ov000_02367d48;

void BossEffect_Create(void *p, s32 id, s32 a, s32 b, s32 c, s32 d);

void BossEffect_CreateHoming(s32 a, s32 b, s32 c, s32 d) {
    BossEffect_Create(&data_ov000_02367d48, 3, a, b, c, d);
}
