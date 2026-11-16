#include <nitro/types.h>

void SpecialStagePlayer_ChangeState(void *p, s32 v);
void SpecialStageCamera_SetMode(s32 v);

void func_ov001_02312de0(void *p) {
    SpecialStagePlayer_ChangeState(p, 0x16);
    SpecialStageCamera_SetMode(5);
}
