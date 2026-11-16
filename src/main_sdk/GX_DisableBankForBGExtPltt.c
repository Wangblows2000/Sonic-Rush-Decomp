#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cff6[];
void GX_DisableBankForBGExtPltt(void) { *(vu32 *)0x4000000 &= ~0x40000000; disableBankForX_(data_0207cff6); }
