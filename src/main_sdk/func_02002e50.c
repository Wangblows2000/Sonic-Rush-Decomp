#include <nitro/types.h>

void func_02002f74(void *p);
extern u8 data_0207cffe[];
void func_02002e50(void) { *(vu32 *)0x4001000 &= ~0x40000000; func_02002f74(data_0207cffe); }
