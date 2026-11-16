#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

s32 func_ov000_022c5f28(void) {
    BOOL b;
    s32 idx;

    b = data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
    if (b)
        idx = 1;
    else
        idx = data_022c4560.unk14;
    if (idx == 1)
        return 8;
    return 6;
}
