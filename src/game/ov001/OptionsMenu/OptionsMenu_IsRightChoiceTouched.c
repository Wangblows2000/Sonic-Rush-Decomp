#include <nitro/types.h>

#pragma thumb on
BOOL OptionsMenu_IsRightChoiceTouched(u32 x, u32 y) {
    if (x >= 0x80 && x < 0xc8 && y >= 0x78 && y < 0x8e) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
