#include <nitro/types.h>

u32 func_020380d8(void *p);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020380a0(void *src, void *dst) { u32 n = func_020380d8(src); MIi_CpuCopy32(src, dst, n); DC_StoreRange(dst, n); }
