#include <nitro/types.h>

typedef struct {
    u8 pad0[0x325];
    u8 unk325;
} UnkStruct_023297c0;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_023297c0_Sys;

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
    u8 pad64[0xE0 - 0x64];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_023297c0_Stage;

extern UnkStruct_023297c0_Sys gGameState;
extern UnkStruct_023297c0_Stage gCamera;

void *func_ov000_023295a4(void *p, s32 dx, s32 b, s32 c, u16 k, s32 d, s32 e);
void *EffWaterSplash_Create(void *p, s32 dx, s32 b, s32 c);

static inline BOOL Stage_Busy(void) {
    if (gCamera.unkE0 != 0 || gCamera.unkE4 != 0)
        return TRUE;
    return FALSE;
}

void EffWaterSplash_Spawn(UnkStruct_023297c0 *p) {
    if (p->unk325 != 0)
        return;
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2) {
        if (Stage_Busy())
            func_ov000_023295a4(p, 0, 0, gCamera.unk62, 0, gCamera.unkE0, gCamera.unkE4);
    } else {
        EffWaterSplash_Create(p, 0, 0, gCamera.unk62);
    }
}
