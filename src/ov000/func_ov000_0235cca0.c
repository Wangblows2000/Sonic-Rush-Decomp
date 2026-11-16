#include <nitro/types.h>

typedef struct UnkStruct_0235cca0 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[4];
    u32 unk10;
} UnkStruct_0235cca0;

extern UnkStruct_0235cca0 gTouchState;

#pragma thumb on
BOOL func_ov000_0235cca0(void) {
    if (gTouchState.unk10 & 0x40) {
        s32 x = gTouchState.unk8;
        s32 y = gTouchState.unkA;
        if (x >= 0x30 && x < 0xb0 && y >= 0x48 && y < 0x60)
            return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
