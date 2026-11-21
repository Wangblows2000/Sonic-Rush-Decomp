/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_WeaponBoss_SelectNextAttack();
s32 ExtraBoss_StepXTowardCameraTarget();

void ExtraBossObj_State_WeaponBossSwordEnd(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x1c8) = (*(u32 *)((u8 *)p0 + 0x1c8) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1c8);
    if (t0 <= 0) {
        ExtraBoss_WeaponBoss_SelectNextAttack(p0, t0);
    }
    ExtraBoss_StepXTowardCameraTarget(p0);
}
