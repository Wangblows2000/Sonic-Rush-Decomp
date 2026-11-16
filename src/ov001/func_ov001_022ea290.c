#include <nitro/types.h>

extern u32 gSystemFlags;
extern u16 gDisplayMain[];

#pragma thumb on
void func_ov001_022ea290(void) {
    gSystemFlags &= ~0x400;
    gDisplayMain[2] = 0;
    gDisplayMain[3] = 0;
}
#pragma thumb reset
