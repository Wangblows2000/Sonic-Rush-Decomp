#include <nitro/types.h>

extern s32 gGameState;
extern u8 data_ov001_023166c4[];
extern u8 data_ov001_023166bc[];

void SysSound_Release(s32 mode);
void Game_StartTimeAttackStage(s32 a, s32 b);
void ZoneMapPause_ExecuteChoice(s32 mode);

#pragma thumb on
void func_ov001_022e96a8(s32 mode, s32 id, s32 b) {
    switch (mode) {
    case 0:
    case 1:
        SysSound_Release(mode);
        if (id < 7) {
            if (gGameState == 0) {
                id = data_ov001_023166c4[id];
            } else {
                id = data_ov001_023166bc[id];
            }
        }
        Game_StartTimeAttackStage(id, b);
        return;
    case 2:
    case 3:
    default:
        ZoneMapPause_ExecuteChoice(mode);
        break;
    }
}
#pragma thumb reset
