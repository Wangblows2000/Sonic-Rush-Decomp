#include <nitro/types.h>

BOOL SpecialStagePlayer_IsGrounded(void *p);
void SpecialStagePlayer_ChangeState(void *p, s32 v);
void SpecialStagePlayer_UpdateAngleFromX(void *p);
s32 SpecialStage_PipeAngleToX(u16 a);
void SpecialStagePlayer_CheckTouchTarget(void *p);

void SpecialStagePlayer_State_AttackRebound(u8 *p) {
    s32 d;
    if (SpecialStagePlayer_IsGrounded(p)) {
        *(s32 *)(p + 0x124) = 0;
        SpecialStagePlayer_ChangeState(p, 5);
        return;
    }
    SpecialStagePlayer_UpdateAngleFromX(p);
    d = SpecialStage_PipeAngleToX(*(s32 *)(p + 0x134)) - *(s32 *)(p + 0x20);
    if (d > 0x1400) {
        d = 0x1400;
    } else if (d < -0x1400) {
        d = -0x1400;
    }
    *(s32 *)(p + 0x20) += d;
    SpecialStagePlayer_CheckTouchTarget(p);
}
