#include <nitro/types.h>

void SaveMsg_Destructor(void) { *(vu16 *)0x4000304 |= 0x8000; }
