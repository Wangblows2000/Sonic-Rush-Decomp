#include <nitro/types.h>

typedef struct {
    u8 pad0[0xe0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_0236aad8 gCamera;

void EffDrownCount_Create(s32 a, s32 b, s32 c, s32 d);
void EffDrownCount_Spawn3D(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void EffDrownCount_Spawn(s32 a, s32 b, s32 c, s32 d) {
    BOOL on;

    if (!(gGameState.unk1C < 9 && gGameState.unk1D < 2)) {
        if (gCamera.unkE0 != 0 || gCamera.unkE4 != 0)
            on = TRUE;
        else
            on = FALSE;
        if (on)
            EffDrownCount_Spawn3D(a, b, c, d, gCamera.unkE0, gCamera.unkE4);
        return;
    }
    EffDrownCount_Create(a, b, c, d);
}
