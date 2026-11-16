#include <nitro/types.h>

void OS_SpinWait(u32 cycles);
void OSi_WaitByLoop(void) { OS_SpinWait(0x1000); }
