#include <nitro/types.h>

void func_02002f74(void *p);
extern u8 data_0207cff6[];
void func_02002f28(void) { *(vu32 *)0x4000000 &= ~0x40000000; func_02002f74(data_0207cff6); }
