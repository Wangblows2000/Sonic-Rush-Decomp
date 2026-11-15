#include <nitro/types.h>

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02039bb0(void *a, void *b);

#pragma thumb on
void func_ov001_022ce374(void **p) {
    vu16 zero;
    if (p[13] != NULL) {
        func_02039bb0(p[0], p[13]);
    }
    zero = 0;
    MIi_CpuClear16(zero, p, 0x48);
}
#pragma thumb reset
