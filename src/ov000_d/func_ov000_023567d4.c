#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padC[4];
    u32 flags;
} UnkTouch_023567d4;

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkPad_023567d4;

extern UnkTouch_023567d4 data_022b65b4;
extern UnkPad_023567d4 data_022503e0;

#pragma thumb on

BOOL func_ov000_023567d4(void) {
    if (data_022b65b4.flags & 0x40) {
        u16 x = data_022b65b4.x;
        u16 y = data_022b65b4.y;
        if (x >= 0x58 && x < 0xb0 && y >= 0x7f && y < 0xaf) {
            return TRUE;
        }
    }
    return (data_022503e0.trg & 1) != 0;
}

#pragma thumb reset
