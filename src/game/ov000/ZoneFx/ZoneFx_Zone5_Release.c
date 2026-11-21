#include <nitro/types.h>

void MIi_CpuCopy32(const void *src, void *dst, u32 size);

void ZoneFx_Zone5_Release(const void *src) {
    MIi_CpuCopy32(src, (void *)0x5000000, 0x200);
    MIi_CpuCopy32(src, (void *)0x5000400, 0x200);
}
