#include <nitro/types.h>

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void ObjVram_Free(void *a, void *b);

#pragma thumb on
void func_ov001_022e13d4(void **p) {
    vu16 zero;
    if (p[13] != NULL) {
        ObjVram_Free(p[0], p[13]);
    }
    zero = 0;
    MIi_CpuClear16(zero, p, 0x48);
}
#pragma thumb reset
