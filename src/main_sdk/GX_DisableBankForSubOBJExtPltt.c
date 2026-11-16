#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207d000[];
void GX_DisableBankForSubOBJExtPltt(void) { *(vu32 *)0x4001000 &= ~0x80000000; disableBankForX_(data_0207d000); }
