#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e9e30;

typedef struct {
    u8 pad0[4];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkPad_022e9e30;

extern UnkTouch_022e9e30 gTouchState;
extern UnkPad_022e9e30 gPad;


#pragma thumb on
BOOL func_ov001_022e9e30(void) {
    if (gTouchState.flags & 0x40) {
        u16 x = gTouchState.x;
        u16 y = gTouchState.y;
        if (x >= 0x60 && x < 0x78 && y >= 0x90 && y < 0xb0) {
            return TRUE;
        }
    }
    if (gPad.rep & 0x20) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
