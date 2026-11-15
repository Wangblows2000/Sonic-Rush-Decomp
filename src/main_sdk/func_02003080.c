#include <nitro/types.h>

void func_02003140(void *p);
extern u8 data_0207cffe[];
void func_02003080(void) { *(vu32 *)0x4001000 &= ~0x40000000; func_02003140(data_0207cffe); }
