#include <nitro/types.h>

void func_02002f74(void *p);
extern u8 data_0207d000[];
void func_02002e28(void) { *(vu32 *)0x4001000 &= ~0x80000000; func_02002f74(data_0207d000); }
