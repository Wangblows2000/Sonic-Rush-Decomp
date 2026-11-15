#include <nitro/types.h>

void func_02002f74(void *p);
extern u8 data_0207cff8[];
void func_02002f04(void) { *(vu32 *)0x4000000 &= ~0x80000000; func_02002f74(data_0207cff8); }
