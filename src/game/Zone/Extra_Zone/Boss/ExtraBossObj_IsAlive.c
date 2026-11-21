#include <nitro/types.h>

BOOL ExtraBossObj_IsAlive(u8 *p) {
    if (p != NULL && !(*(u32 *)(p + 0x14) & 8)) {
        return TRUE;
    }
    return FALSE;
}
