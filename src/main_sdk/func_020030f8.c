#include <nitro/types.h>

void func_02003140(void *p);
extern u8 data_0207cff8[];
void func_020030f8(void) { *(vu32 *)0x4000000 &= ~0x80000000; func_02003140(data_0207cff8); }
