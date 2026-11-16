#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e37bc;

typedef struct {
    u8 pad0[4];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkPad_022e37bc;

extern UnkTouch_022e37bc data_022b65b4;
extern UnkPad_022e37bc data_022503e0;


#pragma thumb on
BOOL func_ov001_022e37bc(void) {
    if (data_022b65b4.flags & 0x40) {
        s32 x = data_022b65b4.x;
        s32 y = data_022b65b4.y;
        if (x >= 8 && x < 0x28 && y >= 0x98 && y < 0xb8) {
            return TRUE;
        }
    } else if (data_022503e0.trg & 2) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
