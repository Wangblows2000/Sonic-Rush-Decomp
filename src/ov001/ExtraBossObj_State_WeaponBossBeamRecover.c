#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBoss_StepXTowardCameraTarget(void *p);
void ExtraBoss_WeaponBoss_SelectNextAttack(void *p);
void ExtraBossObj_State_WeaponBossBeamPause(void);

void ExtraBossObj_State_WeaponBossBeamRecover(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    ExtraBoss_StepXTowardCameraTarget(p);
    if (!(q[0x10c] & 2)) {
        return;
    }
    *(s32 *)(p + 0x1cc) -= 1;
    if (*(s32 *)(p + 0x1cc) <= 0) {
        ExtraBoss_WeaponBoss_SelectNextAttack(p);
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_WeaponBossBeamPause;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 0, 0);
    q[0x10c] |= 4;
    *(s32 *)(p + 0x1c8) = 0x28;
}
