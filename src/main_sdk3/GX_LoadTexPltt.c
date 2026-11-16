#include <nitro/types.h>

extern u32 data_0207d008;
extern u32 data_02077964;

void MI_DmaCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size, void *callback, void *arg);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);

static inline void func_02004ec0_DmaCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size, void *callback, void *arg)
{
    if (dmaNo != (u32)-1) {
        MI_DmaCopy32Async(dmaNo, src, dest, size, callback, arg);
    } else {
        MIi_CpuCopy32(src, dest, size);
    }
}

void GX_LoadTexPltt(const void *pSrc, u32 offset, u32 szByte)
{
    func_02004ec0_DmaCopy32Async(data_02077964, pSrc, (void *)(data_0207d008 + offset), szByte, NULL, NULL);
}
