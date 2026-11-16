#include <nitro/types.h>

extern u32 gTouchState[];
BOOL Records_Unk_ov001_022ece30(s32 a);

#pragma thumb on
BOOL Records_Unk_ov001_022ece98(s32 a) {
    if (gTouchState[4] & 0x40) {
        return Records_Unk_ov001_022ece30(a);
    }
    return FALSE;
}
#pragma thumb reset
