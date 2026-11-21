#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
void OS_Halt(void);
void OS_Terminate(void) { while (1) { (void)OS_DisableInterrupts(); OS_Halt(); } }
