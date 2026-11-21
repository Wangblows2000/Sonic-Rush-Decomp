#include <nitro/types.h>

void OS_UnlockMutex(void *p);
extern u8 data_0207d3e0[];
void SNDi_UnlockMutex(void) { OS_UnlockMutex(data_0207d3e0); }
