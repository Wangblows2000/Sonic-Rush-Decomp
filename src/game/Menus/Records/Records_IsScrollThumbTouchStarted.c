#include <nitro/types.h>

extern u32 gTouchState[];
BOOL Records_IsTouchOnScrollThumb(s32 a);

#pragma thumb on
BOOL Records_IsScrollThumbTouchStarted(s32 a) {
    if (gTouchState[4] & 0x40) {
        return Records_IsTouchOnScrollThumb(a);
    }
    return FALSE;
}
#pragma thumb reset
