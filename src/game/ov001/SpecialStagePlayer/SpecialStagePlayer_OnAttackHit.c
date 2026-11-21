#include <nitro/types.h>

void SpecialStagePlayer_UpdateAngleFromX(void *p);
void SpecialStagePlayer_ChangeState(void *p, s32 v);

void SpecialStagePlayer_OnAttackHit(void *p) {
    SpecialStagePlayer_UpdateAngleFromX(p);
    SpecialStagePlayer_ChangeState(p, 0x13);
}
