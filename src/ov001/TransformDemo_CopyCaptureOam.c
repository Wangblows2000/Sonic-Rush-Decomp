#include <nitro/types.h>

extern u8 data_020885ac[];
void MIi_CpuCopy32(const void *src, void *dst, u32 size);

void TransformDemo_CopyCaptureOam(u8 *p) {
    MIi_CpuCopy32(p + 0x54, data_020885ac, 0x400);
}
