#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padC[4];
    u32 flags;
} UnkTouch_02356794;

typedef struct {
    u16 cont;
} UnkPad_02356794;

extern UnkTouch_02356794 gTouchState;
extern UnkPad_02356794 gPad;

#pragma thumb on

BOOL PlayerSelectMenu_IsLeftInput(void) {
    if (gTouchState.flags & 0x40) {
        u16 x = gTouchState.x;
        u16 y = gTouchState.y;
        if (x >= 0x30 && x < 0x50 && y >= 0x96 && y < 0xb6) {
            return TRUE;
        }
    }
    if (gPad.cont & 0x20) {
        return TRUE;
    }
    return FALSE;
}

#pragma thumb reset
