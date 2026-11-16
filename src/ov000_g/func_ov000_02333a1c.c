#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 unk8;
} UnkStruct_02333a1c_S;

typedef struct {
    u8 pad0[0x90];
    UnkStruct_02333a1c_S unk90;
} UnkStruct_02333a1c_Q;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x328 - 0x18];
    s16 unk328;
    u8 pad32A[2];
    u32 unk32C;
} UnkStruct_02333a1c_P;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_02333a1c_P *unk88;
    u8 pad8C[0x94 - 0x8C];
    UnkStruct_02333a1c_S unk94;
    u8 pad[0x12C - 0x94 - sizeof(UnkStruct_02333a1c_S)];
    UnkStruct_02333a1c_Q *unk12C;
} UnkStruct_02333a1c;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_02333a1c_Sys;

typedef struct {
    u8 pad0[0xE0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_02333a1c_Stage;

extern UnkStruct_02333a1c_Sys gGameState;
extern UnkStruct_02333a1c_Stage gCamera;

void StageObj_SetAnimation(void *w, s32 a);

static inline BOOL Stage_Busy(void) {
    if (gCamera.unkE0 != 0 || gCamera.unkE4 != 0)
        return TRUE;
    return FALSE;
}

void func_ov000_02333a1c(UnkStruct_02333a1c *w) {
    UnkStruct_02333a1c_S *s;
    UnkStruct_02333a1c_P *p = w->unk88;

    if (w->unk12C != NULL)
        s = &w->unk12C->unk90;
    else
        s = &w->unk94;
    if (p == NULL)
        return;
    if (s->unk8 == 1 || s->unk8 == 3) {
        if (w->unk5C & 8)
            w->unk54 |= 4;
        return;
    }
    if ((gGameState.unk1C < 9 && gGameState.unk1D < 2) || !Stage_Busy()) {
        w->unk10 = p->unk10;
        w->unk14 = w->unk88->unk14;
        if (w->unk5C & 1)
            w->unk10 -= w->unk2E << 8;
        else
            w->unk10 += w->unk2E << 8;
        w->unk14 += w->unk30 << 8;
    }
    if (p->unk328 >= 0x32 && p->unk328 <= 0x33 && (p->unk32C & 1))
        return;
    if (s->unk8 == 0)
        StageObj_SetAnimation(w, 1);
    else
        StageObj_SetAnimation(w, 3);
    if (p->unk328 >= 0x32 && p->unk328 <= 0x33)
        return;
    w->unk58 &= ~0x2000;
    w->unk2E = -0x100;
    w->unk30 = 0;
    if (w->unk5C & 1)
        w->unk2E = -w->unk2E;
}
