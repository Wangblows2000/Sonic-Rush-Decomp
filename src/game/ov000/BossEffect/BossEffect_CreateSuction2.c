/* paramclone of BossEffect_CreateBeamFinB (src/ov000/BossEffect_CreateBeamFinB.c); immediates and relocations remapped. */
#include <nitro/types.h>

extern u8 data_ov000_02367d6c;

void BossEffect_Create(void *p, s32 id, s32 a, s32 b, s32 c, s32 d);

void BossEffect_CreateSuction2(s32 a, s32 b, s32 c, s32 d) {
    BossEffect_Create(&data_ov000_02367d6c, 10, a, b, c, d);
}
