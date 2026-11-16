#include <nitro/types.h>

void SpecialStagePlayer_ChangeState(void *p, s32 v);
void SpecialStageCamera_SetMode(s32 v);

void SpecialStagePlayer_Action_Fail(void *p) {
    SpecialStagePlayer_ChangeState(p, 0x16);
    SpecialStageCamera_SetMode(5);
}
