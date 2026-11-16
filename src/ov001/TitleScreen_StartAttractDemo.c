#include <nitro/types.h>

extern u16 gGameState[];
void SceneChange_ToDemoPlay(void);

#pragma thumb on
void TitleScreen_StartAttractDemo(void) {
    gGameState[0xf]++;
    if (gGameState[0xf] >= 4) {
        gGameState[0xf] = 0;
    }
    SceneChange_ToDemoPlay();
}
#pragma thumb reset
