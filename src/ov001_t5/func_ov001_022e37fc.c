#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022e37fc;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022e37fc;

extern UnkTouch_022e37fc data_022b65b4;
extern UnkPad_022e37fc data_022503e0;

#pragma thumb on
BOOL func_ov001_022e37fc(void) {
    s32 x;
    s32 y;

    if (data_022b65b4.flags & 0x40) {
        x = data_022b65b4.x;
        y = data_022b65b4.y;
        if (x >= 8 && x < 0x28 && y >= 0x98 && y < 0xb8) {
            return TRUE;
        }
    } else if (data_022503e0.trg & 0x400) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
