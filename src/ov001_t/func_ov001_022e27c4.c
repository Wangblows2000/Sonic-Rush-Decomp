#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1c;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

BOOL ZoneMap_IsBlaze(void);
s32 func_ov001_022e28ac(void);

#pragma thumb on
s32 func_ov001_022e27c4(void) {
    if (!ZoneMap_IsBlaze()) {
        switch (data_022c4560.unk1c) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        case 3: return 6;
        case 4: return 8;
        case 5: return 12;
        case 6: return 13;
        case 7: return 14;
        }
        return func_ov001_022e28ac();
    } else {
        switch (data_022c4560.unk1c) {
        case 0: return 2;
        case 1: return 6;
        case 2: return 3;
        case 3: return 1;
        case 4: return 12;
        case 5: return 8;
        case 6: return 13;
        case 7: return 14;
        }
        return func_ov001_022e28ac();
    }
}
#pragma thumb reset
