#include <nitro/types.h>

extern u32 data_ov000_0236a65c;

s32 func_ov000_02326498(void) {
    u8 i;

    for (i = 0; i < 32; i++) {
        if (!(data_ov000_0236a65c & (1 << i))) {
            data_ov000_0236a65c |= 1 << i;
            return i;
        }
    }
    return i;
}
