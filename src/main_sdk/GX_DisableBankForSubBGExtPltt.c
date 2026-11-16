#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cffe[];
void GX_DisableBankForSubBGExtPltt(void) { *(vu32 *)0x4001000 &= ~0x40000000; disableBankForX_(data_0207cffe); }
