#include <nitro/types.h>

typedef struct UnkStruct_0235cc74 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[4];
    u32 unk10;
} UnkStruct_0235cc74;

extern UnkStruct_0235cc74 data_022b65b4;

#pragma thumb on
BOOL func_ov000_0235cc74(void) {
    if (data_022b65b4.unk10 & 0x40) {
        s32 x = data_022b65b4.unk8;
        s32 y = data_022b65b4.unkA;
        if (x >= 0x30 && x < 0xb0 && y >= 0x68 && y < 0x80)
            return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
