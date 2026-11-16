#include <nitro/types.h>

typedef struct {
    u8 pad0[0x325];
    u8 unk325;
} UnkStruct_023294c8;

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
    u8 pad64[0xe0 - 0x64];
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

void *EffRipple3D_Spawn(UnkStruct_023294c8 *s, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void EffRipple3D_SpawnForPlayer(UnkStruct_023294c8 *s) {
    BOOL on;

    if (s->unk325 != 0)
        return;
    if (gGameState.unk1C < 9 && gGameState.unk1D < 2)
        return;
    if (gCamera.unkE0 != 0 || gCamera.unkE4 != 0)
        on = TRUE;
    else
        on = FALSE;
    if (on)
        EffRipple3D_Spawn(s, 0, 1, 0, gCamera.unk62, gCamera.unkE0, gCamera.unkE4);
}
