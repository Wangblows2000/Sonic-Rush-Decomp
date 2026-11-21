#include <nitro/types.h>

#pragma thumb on
s32 Records_PrevPageStart(s32 unused, s32 v) {
    s32 m = v % 16;
    if (m == 0) {
        v -= 16;
        if (v < 0) {
            v = 0;
        }
    } else {
        v -= m;
        if (v < 0) {
            v = 0;
        }
    }
    return v;
}
#pragma thumb reset
