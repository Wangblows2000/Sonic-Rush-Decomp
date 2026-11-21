#include <nitro/types.h>

typedef struct { u8 pad[8]; u16 x; u16 y; u8 pad2[4]; u32 flags; } UnkStruct_022b65b4;
extern UnkStruct_022b65b4 gTouchState;

#pragma thumb on
BOOL VsMenu_IsBackButtonTouched(void) {
    if (gTouchState.flags & 0x40) {
        s32 x = gTouchState.x;
        s32 y = gTouchState.y;
        if (x >= 0x10 && x < 0x30 && y >= 0x48 && y < 0x68) {
            return TRUE;
        }
    }
    return FALSE;
}
#pragma thumb reset
