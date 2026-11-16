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

extern UnkTouch_0235fa34 data_022b65b4;
extern UnkPad_0235fa34 data_022503e0;

#pragma thumb on

BOOL func_ov000_0235fa34(void) {
    if (data_022b65b4.flags & 0x40) {
        u16 x = data_022b65b4.x;
        u16 y = data_022b65b4.y;
        if (x >= 0x78 && x < 0xe0 && y >= 0x90 && y < 0xb0) {
            return TRUE;
        }
    }
    return (data_022503e0.trg & 1) != 0;
}

#pragma thumb reset
