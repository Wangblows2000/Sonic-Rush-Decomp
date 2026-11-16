#include <nitro/types.h>

#pragma thumb on
BOOL OptionsMenu_Unk_ov001_022c8b90(u32 x, u32 y) {
    if (x >= 0x38 && x < 0xc8 && y >= 0x60 && y < 0x76) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
