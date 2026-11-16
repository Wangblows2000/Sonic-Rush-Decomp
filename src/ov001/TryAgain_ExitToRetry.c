#include <nitro/types.h>

void SysSound_Release(void);
void Game_StartTimeAttackStage(s32 a, s32 b);

#pragma thumb on
void TryAgain_ExitToRetry(void) {
    SysSound_Release();
    Game_StartTimeAttackStage(-1, -1);
}
#pragma thumb reset
