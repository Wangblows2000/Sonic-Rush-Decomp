#include <nitro/types.h>

void SysSound_Release(void);
void Game_StartTimeAttackStage(s32 a, s32 b);

#pragma thumb on
void func_ov001_022d3434(void) {
    SysSound_Release();
    Game_StartTimeAttackStage(-1, -1);
}
#pragma thumb reset
