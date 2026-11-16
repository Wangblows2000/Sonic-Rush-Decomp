#include <nitro/types.h>

void Game_StartStoryStage(s32 a, s32 b);

#pragma thumb on
void func_ov001_022e19bc(s32 unused, s32 a, s32 b) {
    if (a >= 7) {
        b = 2;
    }
    Game_StartStoryStage(a, b);
}
#pragma thumb reset
