#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x34 - 0x18];
    s16 unk34;
} UnkStruct_02334550_P;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_02334550_P *unk88;
} UnkStruct_02334550;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_02334550_Sys;

typedef struct {
    u8 pad0[0xE0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_02334550_Stage;

extern UnkStruct_02334550_Sys gGameState;
extern UnkStruct_02334550_Stage gCamera;

static inline BOOL Stage_Busy(void) {
    if (gCamera.unkE0 != 0 || gCamera.unkE4 != 0)
        return TRUE;
    return FALSE;
}

void func_ov000_02334550(UnkStruct_02334550 *w) {
    UnkStruct_02334550_P *p = w->unk88;

    if (p != NULL && ((gGameState.unk1C < 9 && gGameState.unk1D < 2) || !Stage_Busy())) {
        w->unk10 = p->unk10;
        w->unk14 = w->unk88->unk14;
        if (w->unk5C & 1)
            w->unk10 -= w->unk2E << 8;
        else
            w->unk10 += w->unk2E << 8;
        w->unk14 += w->unk30 << 8;
        if (w->unk88->unk34 != 0)
            w->unk88 = NULL;
    }
    if (w->unk5C & 8)
        w->unk54 |= 4;
}
