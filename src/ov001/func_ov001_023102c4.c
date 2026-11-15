#include <nitro/types.h>

BOOL func_ov001_023102c4(u32 x) {
    if (x <= 0x4000 || (x >= 0xc000 && (s32)x <= 0x10000)) {
        return TRUE;
    }
    return FALSE;
}
