#include <nitro/types.h>

extern u16 gDisplayMain[];
extern u16 gDisplaySub[];

#pragma thumb on
void func_ov001_022d35b4(void) {
    gDisplayMain[2] += 2;
    gDisplaySub[2] += 2;
}
#pragma thumb reset
