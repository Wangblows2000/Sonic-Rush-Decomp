#include <nitro/types.h>

extern u8 *gDisplayTable[];
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void ObjVram_Reset(s32 i) { vu16 v = 0; MIi_CpuClear16(v, gDisplayTable[i] + 0x98, 0x800); }
