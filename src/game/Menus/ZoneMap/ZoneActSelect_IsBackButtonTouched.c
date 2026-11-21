#include <nitro/types.h>

#pragma thumb on
BOOL ZoneActSelect_IsBackButtonTouched(s32 x, s32 y) {
    if (x >= 0xd8 && x < 0xf8 && y >= 0x98 && y < 0xb8) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
