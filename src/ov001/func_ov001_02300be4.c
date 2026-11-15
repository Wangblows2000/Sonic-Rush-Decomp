#include <nitro/types.h>

BOOL func_ov001_02300be4(u8 *p) {
    if (p != NULL && !(*(u32 *)(p + 0x14) & 8)) {
        return TRUE;
    }
    return FALSE;
}
