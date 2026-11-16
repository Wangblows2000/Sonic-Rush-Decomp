#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e9e70;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022e9e70;

typedef struct {
    u8 pad0[0xe4];
    s32 unke4;
} UnkStruct_022e9e70;

extern UnkTouch_022e9e70 gTouchState;
extern UnkPad_022e9e70 gPad;

#pragma thumb on
BOOL TimeAttackMenu_Unk_ov001_022e9e70(UnkStruct_022e9e70 *p) {
    u16 x;
    u16 y;

    if (gTouchState.flags & 0x40) {
        x = gTouchState.x;
        y = gTouchState.y;
        if (x >= 8 && x < 0x28 && y >= 0x90 && y < 0xb0) {
            p->unke4 = 1;
            return TRUE;
        }
    }
    if (gPad.trg & 2) {
        p->unke4 = 0;
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
