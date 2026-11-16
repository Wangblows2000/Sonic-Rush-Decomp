#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padC[4];
    u32 flags;
} UnkTouch_02356754;

typedef struct {
    u16 cont;
} UnkPad_02356754;

extern UnkTouch_02356754 data_022b65b4;
extern UnkPad_02356754 data_022503e0;

#pragma thumb on

BOOL func_ov000_02356754(void) {
    if (data_022b65b4.flags & 0x40) {
        u16 x = data_022b65b4.x;
        u16 y = data_022b65b4.y;
        if (x >= 0xb0 && x < 0xd0 && y >= 0x96 && y < 0xb6) {
            return TRUE;
        }
    }
    if (data_022503e0.cont & 0x10) {
        return TRUE;
    }
    return FALSE;
}

#pragma thumb reset
