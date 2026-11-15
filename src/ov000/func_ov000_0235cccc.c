#include <nitro/types.h>

typedef struct UnkStruct_0235cccc {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[4];
    u32 unk10;
} UnkStruct_0235cccc;

extern UnkStruct_0235cccc data_022b65b4;

#pragma thumb on
BOOL func_ov000_0235cccc(void) {
    if (data_022b65b4.unk10 & 0x40) {
        s32 x = data_022b65b4.unk8;
        s32 y = data_022b65b4.unkA;
        if (x >= 0x30 && x < 0xb0 && y >= 0x28 && y < 0x40)
            return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
