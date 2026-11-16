#include <nitro/types.h>

extern u8 data_022c18a0;
void OS_WakeupThreadDirect(void *p);
void func_0203db90(void) { OS_WakeupThreadDirect(&data_022c18a0); }
