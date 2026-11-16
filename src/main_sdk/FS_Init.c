#include <nitro/types.h>

extern u32 data_0207efb8;
void FSi_InitRom(u32 a);
void FS_Init(u32 a) { if (data_0207efb8 != 0) return; data_0207efb8 = 1; FSi_InitRom(a); }
