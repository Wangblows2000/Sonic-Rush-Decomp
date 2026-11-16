#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
    u8 pad4[0x10 - 4];
    u32 flags;
} UnkTouch_022ecf04;

typedef struct {
    u16 trg;
} UnkPad_022ecf04;

typedef struct {
    u8 pad0[0x2c];
    s32 active;
    s32 cur;
    s32 max;
} UnkStruct_022ecf04;

extern UnkTouch_022ecf04 gTouchState;
extern UnkPad_022ecf04 gPad;

#pragma thumb on
BOOL Records_Unk_ov001_022ecf04(UnkStruct_022ecf04 *p) {
    s32 x;
    s32 y;

    if (p->active == 0) {
        return FALSE;
    }
    if (p->cur == p->max) {
        return FALSE;
    }
    if (gTouchState.flags & 0x10) {
        x = gTouchState.x;
        y = gTouchState.y;
        if (x >= 0xd8 && x < 0xee && y >= 0x78 && y < 0x8f) {
            return TRUE;
        }
    }
    if (gPad.trg & 0x80) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
