#include <nitro/types.h>

BOOL SpecialStagePlayer_IsGrounded(void *p);
void SpecialStagePlayer_ChangeState(void *p, s32 v);

void SpecialStagePlayer_State_AttackFall(void *p) {
    if (!SpecialStagePlayer_IsGrounded(p)) {
        return;
    }
    SpecialStagePlayer_ChangeState(p, 5);
}
