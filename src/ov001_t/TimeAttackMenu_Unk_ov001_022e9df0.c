#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e9df0;

typedef struct {
    u8 pad0[4];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkPad_022e9df0;

extern UnkTouch_022e9df0 gTouchState;
extern UnkPad_022e9df0 gPad;


#pragma thumb on
BOOL TimeAttackMenu_Unk_ov001_022e9df0(void) {
    if (gTouchState.flags & 0x40) {
        u16 x = gTouchState.x;
        u16 y = gTouchState.y;
        if (x >= 0xe2 && x < 0xfa && y >= 0x90 && y < 0xb0) {
            return TRUE;
        }
    }
    if (gPad.rep & 0x10) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
