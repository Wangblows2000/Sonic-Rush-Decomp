#include <nitro/types.h>

typedef struct {
    s32 w0;
    u8 pad4[0xc];
    s32 w10;
    s32 w14;
    s32 w18;
    u8 b1c;
    u8 b1d;
    u16 h1e;
} UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void SceneChange_ToPlayerSelectMenuMode4(void);

#pragma thumb on
void TitleMenu_ExitToPlayerSelectMode4(void) {
    gGameState.w10 = 2;
    gGameState.w18 &= ~0x20;
    gGameState.w18 &= ~0x10;
    gGameState.b1c = 0;
    gGameState.b1d = 0;
    SceneChange_ToPlayerSelectMenuMode4();
}
#pragma thumb reset
