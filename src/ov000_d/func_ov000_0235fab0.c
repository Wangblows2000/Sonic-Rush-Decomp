#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padC[4];
    u32 flags;
} UnkTouch_0235fab0;

typedef struct {
    u16 cont;
} UnkPad_0235fab0;

extern UnkTouch_0235fab0 data_022b65b4;
extern UnkPad_0235fab0 data_022503e0;

#pragma thumb on

BOOL func_ov000_0235fab0(void) {
    if (data_022b65b4.flags & 0x40) {
        u16 x = data_022b65b4.x;
        u16 y = data_022b65b4.y;
        if (x >= 0x60 && x < 0x78 && y >= 0x90 && y < 0xb0) {
            return TRUE;
        }
    }
    if (data_022503e0.cont & 0x20) {
        return TRUE;
    }
    return FALSE;
}

#pragma thumb reset
