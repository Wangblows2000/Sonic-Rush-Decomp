#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e9ebc;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022e9ebc;

typedef struct {
    u8 pad0[4];
    s32 mode;
} UnkStruct_022e9ebc;

extern UnkTouch_022e9ebc gTouchState;
extern UnkPad_022e9ebc gPad;

#pragma thumb on
BOOL TimeAttackMenu_Unk_ov001_022e9ebc(UnkStruct_022e9ebc *p) {
    u16 x;
    u16 y;

    if (p->mode == 2 || p->mode == 3) {
        return FALSE;
    }
    if (gTouchState.flags & 0x40) {
        x = gTouchState.x;
        y = gTouchState.y;
        if (x >= 0x78 && x < 0xe0 && y >= 0x90 && y < 0xb0) {
            return TRUE;
        }
    }
    if (gPad.trg & 1) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
