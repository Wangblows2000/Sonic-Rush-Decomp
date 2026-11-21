#include <nitro/types.h>

extern void *data_ov001_0231f068;
extern u32 gRandSeed;
extern s32 data_ov001_0231693c[];
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBoss_StopAndFaceCenter2(void *p);
void ExtraBossObj_State_WeaponBossWait(void);
static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void ExtraBoss_WeaponBoss_Action_Wait(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_WeaponBossWait;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 0, 0);
    q[0x10c] |= 4;
    *(s32 *)(q + 0x114) = 0x1000;
    *(s32 *)(p + 0x1c8) = data_ov001_0231693c[Rand() & 3] + (Rand() & 0x3f);
    ExtraBoss_StopAndFaceCenter2(p);
}
