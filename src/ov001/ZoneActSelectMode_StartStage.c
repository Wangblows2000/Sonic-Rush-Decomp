#include <nitro/types.h>

extern u32 gGameState;
void Game_StartStoryStage(u32 a, u32 b);

#pragma thumb on
void ZoneActSelectMode_StartStage(u32 a) {
    gGameState = 0;
    Game_StartStoryStage(0, a);
}
#pragma thumb reset
