#include <nitro/types.h>

void OS_SpinWait(u32 cycles);
void func_02005628(void) { OS_SpinWait(0x1000); }
