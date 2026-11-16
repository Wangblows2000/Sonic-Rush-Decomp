#include <nitro/types.h>

typedef struct {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_022c4590;

extern UnkStruct_022c4590 data_022c4590;

BOOL func_ov000_0235cbd8(void);

#pragma thumb on

s32 func_ov000_0235cbf0(void) {
    if (func_ov000_0235cbd8()) {
        switch (data_022c4590.unk40) {
        case 1:
            return 0;
        case 2:
            return 1;
        default:
            return 2;
        }
    } else {
        switch (data_022c4590.unk40) {
        case 1:
            return 1;
        case 2:
            return 0;
        default:
            return 2;
        }
    }
}

#pragma thumb reset
