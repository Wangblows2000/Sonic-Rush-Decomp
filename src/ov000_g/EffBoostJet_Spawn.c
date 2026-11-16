#include <nitro/types.h>

typedef struct {
    u8 pad0[0x324];
    u8 unk324;
    u8 unk325;
    u8 pad326[0x330 - 0x326];
    u32 unk330;
} UnkStruct_02331a24;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_02331a24_Sys;

typedef struct {
    u8 pad0[0xE0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_02331a24_Stage;

extern UnkStruct_02331a24_Sys gGameState;
extern UnkStruct_02331a24_Stage gCamera;

void *func_ov000_023315f4(void *a, s32 b, s32 c, s32 d, s32 e);
void *EffBoostJet3D_Create(void *a, s32 b, s32 c);
void *EffBoostJet_Create(void *a, s32 b, s32 c);

static inline BOOL Stage_Busy(void) {
    if (gCamera.unkE0 != 0 || gCamera.unkE4 != 0)
        return TRUE;
    return FALSE;
}

void EffBoostJet_Spawn(UnkStruct_02331a24 *p) {
    if (p->unk325 != 0)
        return;
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2) {
        if (Stage_Busy())
            func_ov000_023315f4(p, -8, 6, gCamera.unkE0, gCamera.unkE4);
        else
            EffBoostJet3D_Create(p, -8, 6);
    } else {
        EffBoostJet_Create(p, -8, 6);
    }
}
