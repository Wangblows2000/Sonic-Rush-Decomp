#include <nitro/types.h>

extern u8 gGameState[];

#pragma thumb on
BOOL TryAgain_Unk_ov001_022d344c(void) {
    if (gGameState[0x1d] <= 1) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
