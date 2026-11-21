#include <nitro/types.h>

void SpecialStagePlayer_ChangeState(void *p, s32 v);

void SpecialStagePlayer_Action_Start(void *p) {
    SpecialStagePlayer_ChangeState(p, 0);
}
