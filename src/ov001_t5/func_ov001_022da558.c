#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022da558;

extern UnkTouch_022da558 data_022b65b4;

#pragma thumb on
s32 func_ov001_022da558(void) {
    s32 x;
    s32 y;
    s32 i;
    s32 top;

    if (data_022b65b4.flags & 0x40) {
        x = data_022b65b4.x;
        y = data_022b65b4.y;
        for (i = 0, top = 0x20; i < 8; top += 0x11, i++) {
            if (x >= 0x38 && x < 0xc0 && y >= top && y < top + 0x10) {
                return i;
            }
        }
    }
    return -1;
}
#pragma thumb reset
