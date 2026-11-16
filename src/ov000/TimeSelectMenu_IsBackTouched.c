#include <nitro/types.h>

typedef struct UnkStruct_0235f9f0 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[4];
    u32 unk10;
} UnkStruct_0235f9f0;

extern UnkStruct_0235f9f0 gTouchState;

#pragma thumb on
BOOL TimeSelectMenu_IsBackTouched(void) {
    if (gTouchState.unk10 & 0x40) {
        u16 x = gTouchState.unk8;
        u16 y = gTouchState.unkA;
        if (x >= 8 && x < 0x28 && y >= 0x90 && y < 0xb0)
            return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
