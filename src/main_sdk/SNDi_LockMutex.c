#include <nitro/types.h>

void OS_LockMutex(void *p);
extern u8 data_0207d3e0[];
void SNDi_LockMutex(void) { OS_LockMutex(data_0207d3e0); }
