#include <nitro/types.h>

extern u16 data_02077960;
extern u16 data_0207cfe4;
void GX_DispOff(void) { u32 v = *(vu32 *)0x4000000; data_02077960 = 0; data_0207cfe4 = (u16)((v & 0x30000) >> 16); *(vu32 *)0x4000000 = v & ~0x30000; }
