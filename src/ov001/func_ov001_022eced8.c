#include <nitro/types.h>

typedef struct { u8 pad[8]; u16 x; u16 y; u8 pad2[4]; u32 flags; } UnkStruct_022b65b4;
extern UnkStruct_022b65b4 data_022b65b4;

#pragma thumb on
BOOL func_ov001_022eced8(void) {
    if (data_022b65b4.flags & 0x40) {
        s32 x = data_022b65b4.x;
        s32 y = data_022b65b4.y;
        if (x >= 0x10 && x < 0x30 && y >= 0x98 && y < 0xb8) {
            return TRUE;
        }
    }
    return FALSE;
}
#pragma thumb reset
