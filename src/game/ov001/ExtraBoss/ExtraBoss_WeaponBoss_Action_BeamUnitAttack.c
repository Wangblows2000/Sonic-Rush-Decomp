#include <nitro/types.h>

extern void *data_ov001_0231f068;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
s32 ExtraBoss_CountActiveBeamUnits(void);
void ExtraBoss_StopAndFaceCenter2(void *p);
void ExtraBossObj_State_WeaponBossBeamUnitWaitClear(void);
void ExtraBossObj_State_WeaponBossBeamUnitAim(void);

void ExtraBoss_WeaponBoss_Action_BeamUnitAttack(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    *(s32 *)(p + 0x1cc) = 2;
    if (ExtraBoss_CountActiveBeamUnits() != 0) {
        *(void (**)(void))(p + 0x74) = ExtraBossObj_State_WeaponBossBeamUnitWaitClear;
        Model_SetAnimation(q, 0, data_ov001_0231f068, 0, 0);
        q[0x10c] |= 4;
        *(s32 *)(q + 0x114) = 0x1000;
        ExtraBoss_StopAndFaceCenter2(p);
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_WeaponBossBeamUnitAim;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 5, 0);
    q[0x10c] &= ~4;
    *(s32 *)(q + 0x114) = 0x1000;
}
