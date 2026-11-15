#include <nitro/types.h>

void func_02003140(void *p);
extern u8 data_0207cff6[];
void func_0200311c(void) { *(vu32 *)0x4000000 &= ~0x40000000; func_02003140(data_0207cff6); }
