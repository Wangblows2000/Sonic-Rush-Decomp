#include <nitro/types.h>

#pragma thumb on
BOOL OptionsMenu_Unk_ov001_022c8ba8(u32 x, u32 y) {
    if (x >= 0xd8 && x < 0xf8 && y >= 0x90 && y < 0xb0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
