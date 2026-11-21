#include <nitro/types.h>

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBoss_StepXTowardCameraTarget(void *p);
void ExtraBoss_SpawnHomingBeam(void *p, s32 a, s32 b, s32 c);
void ExtraBossObj_State_WeaponBossBeamFire(void);

void ExtraBossObj_State_WeaponBossBeamCharge(u8 *p) {
    u8 *r = *(u8 **)(p + 0x84);
    u8 *q;
    ExtraBoss_StepXTowardCameraTarget(p);
    if (!(r[0x10c] & 2)) {
        return;
    }
    q = *(u8 **)(p + 0x1dc);
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_WeaponBossBeamFire;
    Model_SetAnimation(r, 0, data_ov001_0231f068, 0x11, 0);
    *(s32 *)(p + 0x1d0) = 3;
    ExtraBoss_SpawnHomingBeam(p, *(s32 *)(q + 0xe0) - data_ov001_0231f524, -(*(s32 *)(q + 0xe4) + data_ov001_0231f520), 0);
}
