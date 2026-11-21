#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u16 n;
} UnkStruct_022e2640;

extern UnkStruct_022e2640 gSaveData;

#pragma thumb on
BOOL ZoneMap_IsSonicZoneSelectable(s32 unused, s32 b) {
    s32 limit = -1;
    s32 n = gSaveData.n;

    if (n >= 0x1a) {
        limit = 6;
    } else if (n >= 0x18) {
        limit = 6;
    } else if (n >= 0x17) {
        limit = 6;
    } else if (n >= 0x15) {
        limit = 5;
    } else if (n >= 0x14) {
        limit = 5;
    } else if (n >= 0x11) {
        limit = 4;
    } else if (n >= 0x10) {
        limit = 4;
    } else if (n >= 0xd) {
        limit = 3;
    } else if (n >= 0xc) {
        limit = 3;
    } else if (n >= 9) {
        limit = 2;
    } else if (n >= 8) {
        limit = 2;
    } else if (n >= 5) {
        limit = 1;
    } else if (n >= 4) {
        limit = 1;
    } else if (n >= 2) {
        limit = 0;
    } else if (n >= 1) {
        limit = 0;
    }
    return b <= limit;
}
#pragma thumb reset
