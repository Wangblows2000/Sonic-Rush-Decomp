#include <nitro/types.h>

#pragma thumb on
BOOL TitleMenu_IsBackButtonTouched(u32 x, u32 y) {
    if (x < 0x10) {
        return FALSE;
    }
    if (x >= 0x30) {
        return FALSE;
    }
    if (y < 0x4c) {
        return FALSE;
    }
    if (y < 0x6c) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
