#include <nitro/types.h>

BOOL MBi_IsCommSizeValid(u32 parentSize, u32 childSize, u32 maxEntry)
{
    if (parentSize > 0x1fe || parentSize < 0xe4) {
        return FALSE;
    }
    if (childSize > 0x10 || childSize < 8) {
        return FALSE;
    }
    return (s32)((parentSize + 0x26) * 4 + 0x14a + ((childSize + 0x20) * 4 + 0x70) * maxEntry) < 0x15e0;
}
