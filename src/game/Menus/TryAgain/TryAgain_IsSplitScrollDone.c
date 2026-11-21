#include <nitro/types.h>

#pragma thumb on
BOOL TryAgain_IsSplitScrollDone(u8 *p) {
    if (*(s32 *)(p + 0x5d4) == 0) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
