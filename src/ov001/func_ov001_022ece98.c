#include <nitro/types.h>

extern u32 data_022b65b4[];
BOOL func_ov001_022ece30(s32 a);

#pragma thumb on
BOOL func_ov001_022ece98(s32 a) {
    if (data_022b65b4[4] & 0x40) {
        return func_ov001_022ece30(a);
    }
    return FALSE;
}
#pragma thumb reset
