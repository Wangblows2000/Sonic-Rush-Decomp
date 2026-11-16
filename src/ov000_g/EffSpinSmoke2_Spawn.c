#include <nitro/types.h>

typedef struct {
    u8 pad0[0x324];
    u8 unk324;
    u8 unk325;
    u8 pad326[0x330 - 0x326];
    u32 unk330;
} UnkStruct_023340f0;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_023340f0_Sys;

typedef struct {
    u8 pad0[0xE0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_023340f0_Stage;

extern UnkStruct_023340f0_Sys gGameState;
extern UnkStruct_023340f0_Stage gCamera;

void *func_ov000_02333bec(void *a, s32 b, s32 c, s32 d, s32 e, u16 f);
void *EffSpinSmoke2_3D_Create(void *a, s32 b, s32 c, u16 d);
void *EffSpinSmoke2_Create(void *a, s32 b, s32 c, u16 d);

static inline BOOL Stage_Busy(void) {
    if (gCamera.unkE0 != 0 || gCamera.unkE4 != 0)
        return TRUE;
    return FALSE;
}

void EffSpinSmoke2_Spawn(UnkStruct_023340f0 *p) {
    if (p->unk325 != 0)
        return;
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2) {
        if (Stage_Busy())
            func_ov000_02333bec(p, -10, 0xe, gCamera.unkE0, gCamera.unkE4, p->unk324);
        else
            EffSpinSmoke2_3D_Create(p, -10, 0xe, p->unk324);
    } else {
        if (p->unk330 & 0x1000)
            return;
        EffSpinSmoke2_Create(p, -10, 0xe, p->unk324);
    }
}
