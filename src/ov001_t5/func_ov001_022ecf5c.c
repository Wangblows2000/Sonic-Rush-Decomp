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

extern UnkTouch_022ecf5c data_022b65b4;
extern UnkPad_022ecf5c data_022503e0;

#pragma thumb on
BOOL func_ov001_022ecf5c(UnkStruct_022ecf5c *p) {
    s32 x;
    s32 y;

    if (p->active == 0) {
        return FALSE;
    }
    if (p->cur == 0) {
        return FALSE;
    }
    if (data_022b65b4.flags & 0x10) {
        x = data_022b65b4.x;
        y = data_022b65b4.y;
        if (x >= 0xd8 && x < 0xee && y >= 0x10 && y < 0x27) {
            return TRUE;
        }
    }
    if (data_022503e0.trg & 0x40) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
