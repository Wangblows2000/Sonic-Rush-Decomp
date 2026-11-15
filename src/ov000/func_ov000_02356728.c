#include <nitro/types.h>

typedef struct UnkStruct_02356728 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[4];
    u32 unk10;
} UnkStruct_02356728;

extern UnkStruct_02356728 data_022b65b4;

#pragma thumb on
BOOL func_ov000_02356728(void) {
    if (data_022b65b4.unk10 & 0x40) {
        u16 x = data_022b65b4.unk8;
        u16 y = data_022b65b4.unkA;
        if (x >= 8 && x < 0x28 && y >= 0x4c && y < 0x6c)
            return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
