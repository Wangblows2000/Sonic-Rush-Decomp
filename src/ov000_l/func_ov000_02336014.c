#include <nitro/types.h>

s32 func_ov000_02336014(s32 v) {
    if (v < 0)
        v = 0;
    else if (v > 50)
        v = 50;
    return ((v << 16) >> 4) / 40 + 0x1800;
}
