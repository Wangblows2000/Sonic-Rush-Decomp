#include <nitro/types.h>

extern u8 *data_02072c78[];
extern void *data_022b4574;
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void func_020371c8(void *p);
void func_02038584(void) { u8 *p = data_02072c78[1]; vu16 v = 0x200; MIi_CpuClear16(v, p + 0xca8, 0x400); *(u16 *)(p + 0x8a0) = 0; func_020371c8(data_022b4574); }
