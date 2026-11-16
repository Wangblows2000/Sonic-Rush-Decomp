#include <nitro/types.h>
#include "main_a2/dual.h"

void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void func_02032d2c(UnkDualSub *p);

void func_02032c8c(UnkDual *d) {
    UnkDualSub *ptrs[2];
    UnkDualSub tmp;
    u32 i;
    ptrs[0] = &d->sub;
    ptrs[1] = &tmp;
    MIi_CpuCopy16(d, &tmp, sizeof(UnkDualSub));
    for (i = 0; i < 2; i++) {
        if (!((1 << i) & d->mask)) {
            UnkDualSub *p = ptrs[i];
            UNK_DUAL_SETUP(p, d, i);
            func_02032d2c(p);
        }
    }
}
