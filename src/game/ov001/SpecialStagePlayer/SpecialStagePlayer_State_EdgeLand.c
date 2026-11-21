#include <nitro/types.h>

void SpecialStagePlayer_ChangeState(void *p, s32 v);

void SpecialStagePlayer_State_EdgeLand(void *p) {
    SpecialStagePlayer_ChangeState(p, 2);
}
