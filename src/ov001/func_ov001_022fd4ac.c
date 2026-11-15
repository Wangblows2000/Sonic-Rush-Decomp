#include <nitro/types.h>

extern u8 *data_ov001_0231f094;

s32 func_ov001_022fd4ac(void) {
    s32 n = 0;
    u8 b = *data_ov001_0231f094;
    if (b & 1) {
        n++;
    }
    if (b & 2) {
        n++;
    }
    return n;
}
