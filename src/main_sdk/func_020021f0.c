#include <nitro/types.h>

u32 FX_GetSqrtResult(void);
u32 func_020021f0(s32 v) {
    if (v > 0) {
        *(vu16 *)0x40002b0 = 1;
        *(volatile u64 *)0x40002b8 = (u64)v << 32;
        return FX_GetSqrtResult();
    }
    return 0;
}
