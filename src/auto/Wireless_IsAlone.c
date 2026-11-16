/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WH_GetBitmap();

s32 Wireless_IsAlone(void) {
    s32 t0 = WH_GetBitmap();
    s32 v1 = 1;
    if ((t0 & ~1) != 0) {
        v1 = 0;
    }
    return v1;
}

#pragma thumb reset
