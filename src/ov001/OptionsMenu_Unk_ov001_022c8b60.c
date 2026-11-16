#include <nitro/types.h>

#pragma thumb on
BOOL OptionsMenu_Unk_ov001_022c8b60(u32 x, u32 y) {
    if (x >= 0x80 && x < 0xc8 && y >= 0x78 && y < 0x8e) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
