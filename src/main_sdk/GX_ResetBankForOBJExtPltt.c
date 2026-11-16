#include <nitro/types.h>

void resetBankForX_(void *p);
extern u8 data_0207cff8[];
void GX_ResetBankForOBJExtPltt(void) { *(vu32 *)0x4000000 &= ~0x80000000; resetBankForX_(data_0207cff8); }
