#include <nitro/types.h>

typedef struct UnkStruct_0235cc74 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[4];
    u32 unk10;
} UnkStruct_0235cc74;

extern UnkStruct_0235cc74 gTouchState;

#pragma thumb on
BOOL ResultsScreen_IsBottomButtonTouched(void) {
    if (gTouchState.unk10 & 0x40) {
        s32 x = gTouchState.unk8;
        s32 y = gTouchState.unkA;
        if (x >= 0x30 && x < 0xb0 && y >= 0x68 && y < 0x80)
            return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
