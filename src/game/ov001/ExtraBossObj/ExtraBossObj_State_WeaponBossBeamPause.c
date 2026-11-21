#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBoss_StepXTowardCameraTarget(void *p);
void ExtraBossObj_State_WeaponBossBeamCharge(void);

void ExtraBossObj_State_WeaponBossBeamPause(u8 *p) {
    u8 *q;
    *(s32 *)(p + 0x1c8) -= 1;
    ExtraBoss_StepXTowardCameraTarget(p);
    if (*(s32 *)(p + 0x1c8) > 0) {
        return;
    }
    q = *(u8 **)(p + 0x84);
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_WeaponBossBeamCharge;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 0x10, 0);
    q[0x10c] &= ~4;
}
