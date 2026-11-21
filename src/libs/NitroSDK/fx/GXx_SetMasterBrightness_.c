#include <nitro/types.h>

void GXx_SetMasterBrightness_(vu16 *reg, s32 b) {
    if (b == 0) *reg = 0;
    else if (b > 0) *reg = (u16)(0x4000 | b);
    else *reg = (u16)(0x8000 | -b);
}
