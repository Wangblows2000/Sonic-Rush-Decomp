#include <nitro/types.h>

#pragma thumb on
void func_ov001_022e35c4(s32 *p, s32 v) {
    if (p[2] != v) {
        p[2] = v;
        p[0] = 0;
    }
}
#pragma thumb reset
