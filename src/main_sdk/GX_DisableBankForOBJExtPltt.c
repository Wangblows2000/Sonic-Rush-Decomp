#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cff8[];
void GX_DisableBankForOBJExtPltt(void) { *(vu32 *)0x4000000 &= ~0x80000000; disableBankForX_(data_0207cff8); }
