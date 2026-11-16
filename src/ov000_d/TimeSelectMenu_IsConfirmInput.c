#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padC[4];
    u32 flags;
} UnkTouch_0235fa34;

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkPad_0235fa34;

extern UnkTouch_0235fa34 gTouchState;
extern UnkPad_0235fa34 gPad;

#pragma thumb on

BOOL TimeSelectMenu_IsConfirmInput(void) {
    if (gTouchState.flags & 0x40) {
        u16 x = gTouchState.x;
        u16 y = gTouchState.y;
        if (x >= 0x78 && x < 0xe0 && y >= 0x90 && y < 0xb0) {
            return TRUE;
        }
    }
    return (gPad.trg & 1) != 0;
}

#pragma thumb reset
