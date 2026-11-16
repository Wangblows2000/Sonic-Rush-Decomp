#include <nitro/types.h>

typedef struct {
    u8 pad0[0x24];
    u16 n;
} UnkStruct_022e24a8;

extern UnkStruct_022e24a8 data_022c46c0;

#pragma thumb on
s32 ZoneMap_Unk_ov001_022e24a8(void) {
    s32 r = -1;
    s32 n = data_022c46c0.n;

    if (n >= 0x1a) {
        r = 6;
    } else if (n >= 0x1a) {
        r = 6;
    } else if (n >= 0x19) {
        r = 6;
    } else if (n >= 0x16) {
        r = 5;
    } else if (n >= 0x15) {
        r = 5;
    } else if (n >= 0x12) {
        r = 4;
    } else if (n >= 0x11) {
        r = 4;
    } else if (n >= 0xe) {
        r = 3;
    } else if (n >= 0xd) {
        r = 3;
    } else if (n >= 0xa) {
        r = 2;
    } else if (n >= 9) {
        r = 2;
    } else if (n >= 7) {
        r = 1;
    } else if (n >= 6) {
        r = 1;
    } else if (n >= 3) {
        r = 0;
    } else if (n >= 2) {
        r = 0;
    }
    return r;
}
#pragma thumb reset
