#include <nitro/types.h>

extern u16 data_0207d3a0;
void OS_Terminate(void);
void func_0200777c(u32 tag, u32 data) { u16 c = (u16)((data & 0x7f00) >> 8); if (c == 0x10) { data_0207d3a0 = 1; return; } OS_Terminate(); }
