#include <nitro/types.h>

u32 GX_VBlankIntr(BOOL en) {
    u32 old = *(vu16 *)0x4000004 & 8;
    if (en) *(vu16 *)0x4000004 |= 8; else *(vu16 *)0x4000004 &= ~8;
    return old;
}
