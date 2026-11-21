#include <nitro/types.h>

extern u32 gTouchState[];
extern u16 gPad[];

#pragma thumb on
BOOL TitleDemo_CheckSkipInput(void) {
    if (gTouchState[4] & 0x40) {
        return TRUE;
    }
    if (gPad[2] & 9) {
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
