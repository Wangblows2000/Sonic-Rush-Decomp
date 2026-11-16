#include <nitro/types.h>

void resetBankForX_(void *p);
extern u8 data_0207d000[];
void GX_ResetBankForSubOBJExtPltt(void) { *(vu32 *)0x4001000 &= ~0x80000000; resetBankForX_(data_0207d000); }
