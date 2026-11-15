#include <nitro/types.h>

#pragma thumb on
void func_ov001_022e3840(u8 *p, s32 v) {
    *(s32 *)(p + 0x10) = v;
}
#pragma thumb reset
