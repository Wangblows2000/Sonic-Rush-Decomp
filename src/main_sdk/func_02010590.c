#include <nitro/types.h>

s32 func_0200e390(void);
void func_0201052c(u32 a, u32 b);
void OS_Terminate(void);
void func_02010590(void) { if ((*(vu16 *)0x27fffa8 & 0x8000) >> 15) func_0200e390(); func_0201052c(1, 1); OS_Terminate(); }
