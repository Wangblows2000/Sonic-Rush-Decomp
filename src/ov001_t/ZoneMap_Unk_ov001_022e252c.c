#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u16 n;
} UnkStruct_022e252c;

extern UnkStruct_022e252c gSaveData;

#pragma thumb on
s32 ZoneMap_Unk_ov001_022e252c(void) {
    s32 r = -1;
    s32 n = gSaveData.n;

    if (n >= 0x1a) {
        r = 6;
    } else if (n >= 0x18) {
        r = 6;
    } else if (n >= 0x17) {
        r = 6;
    } else if (n >= 0x15) {
        r = 5;
    } else if (n >= 0x14) {
        r = 5;
    } else if (n >= 0x11) {
        r = 4;
    } else if (n >= 0x10) {
        r = 4;
    } else if (n >= 0xd) {
        r = 3;
    } else if (n >= 0xc) {
        r = 3;
    } else if (n >= 9) {
        r = 2;
    } else if (n >= 8) {
        r = 2;
    } else if (n >= 5) {
        r = 1;
    } else if (n >= 4) {
        r = 1;
    } else if (n >= 2) {
        r = 0;
    } else if (n >= 1) {
        r = 0;
    }
    return r;
}
#pragma thumb reset
