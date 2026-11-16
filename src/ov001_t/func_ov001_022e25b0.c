#include <nitro/types.h>

typedef struct {
    u8 pad0[0x24];
    u16 n;
} UnkStruct_022e25b0;

extern UnkStruct_022e25b0 data_022c46c0;

#pragma thumb on
BOOL func_ov001_022e25b0(s32 unused, s32 b) {
    s32 limit = -1;
    s32 n = data_022c46c0.n;

    if (n >= 0x1a) {
        limit = 6;
    } else if (n >= 0x1a) {
        limit = 6;
    } else if (n >= 0x19) {
        limit = 6;
    } else if (n >= 0x16) {
        limit = 5;
    } else if (n >= 0x15) {
        limit = 5;
    } else if (n >= 0x12) {
        limit = 4;
    } else if (n >= 0x11) {
        limit = 4;
    } else if (n >= 0xe) {
        limit = 3;
    } else if (n >= 0xd) {
        limit = 3;
    } else if (n >= 0xa) {
        limit = 2;
    } else if (n >= 9) {
        limit = 2;
    } else if (n >= 7) {
        limit = 1;
    } else if (n >= 6) {
        limit = 1;
    } else if (n >= 3) {
        limit = 0;
    } else if (n >= 2) {
        limit = 0;
    }
    return b <= limit;
}
#pragma thumb reset
