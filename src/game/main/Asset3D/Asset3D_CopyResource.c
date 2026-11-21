#include <nitro/types.h>

u32 Asset3D_GetResourceSize(void *p);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void Asset3D_CopyResource(void *src, void *dst) { u32 n = Asset3D_GetResourceSize(src); MIi_CpuCopy32(src, dst, n); DC_StoreRange(dst, n); }
