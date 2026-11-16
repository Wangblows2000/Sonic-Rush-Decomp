#include <nitro/types.h>

extern u32 gSystemFlags;

#pragma thumb on
void func_ov001_022e8328(void) {
    gSystemFlags &= ~0x400;
}
#pragma thumb reset
