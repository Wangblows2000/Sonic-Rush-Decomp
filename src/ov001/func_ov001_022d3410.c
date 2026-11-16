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
void SceneChange_ToPlayerSelectMenuMode3(void);
void SceneChange_ToPlayerSelectMenuMode4(void);

#pragma thumb on
void func_ov001_022d3410(void) {
    if (gGameState.b1d < 2) {
        SceneChange_ToPlayerSelectMenuMode3();
    } else {
        SceneChange_ToPlayerSelectMenuMode4();
    }
}
#pragma thumb reset
