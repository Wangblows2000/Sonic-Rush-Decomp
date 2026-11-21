#include <nitro/types.h>

u32 OS_EnableInterrupts(void);
void OS_Halt(void);
void OSi_IdleThreadProc(void *arg) { (void)OS_EnableInterrupts(); while (1) OS_Halt(); }
