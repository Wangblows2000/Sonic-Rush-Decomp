#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
    u8 pad4[0x10 - 4];
    u32 flags;
} UnkTouch_022ecf5c;

typedef struct {
    u16 trg;
} UnkPad_022ecf5c;

typedef struct {
    u8 pad0[0x2c];
    s32 active;
    s32 cur;
    s32 max;
} UnkStruct_022ecf5c;

extern UnkTouch_022ecf5c gTouchState;
extern UnkPad_022ecf5c gPad;

#pragma thumb on
BOOL Records_IsUpArrowTouched(UnkStruct_022ecf5c *p) {
    s32 x;
    s32 y;

    if (p->active == 0) {
        return FALSE;
    }
    if (p->cur == 0) {
        return FALSE;
    }
    if (gTouchState.flags & 0x10) {
        x = gTouchState.x;
        y = gTouchState.y;
        if (x >= 0xd8 && x < 0xee && y >= 0x10 && y < 0x27) {
            return TRUE;
        }
    }
    if (gPad.trg & 0x40) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
