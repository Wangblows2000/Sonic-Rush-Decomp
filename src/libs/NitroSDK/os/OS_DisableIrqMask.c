#include <nitro/types.h>

u32 OS_DisableIrqMask(u32 mask) {
    u16 ime = *(vu16 *)0x4000208;
    u32 old;
    *(vu16 *)0x4000208 = 0;
    old = *(vu32 *)0x4000210;
    *(vu32 *)0x4000210 = old & ~mask;
    (void)*(vu16 *)0x4000208;
    *(vu16 *)0x4000208 = ime;
    return old;
}
