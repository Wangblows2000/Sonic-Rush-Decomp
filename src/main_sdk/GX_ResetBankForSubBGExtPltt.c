#include <nitro/types.h>

void resetBankForX_(void *p);
extern u8 data_0207cffe[];
void GX_ResetBankForSubBGExtPltt(void) { *(vu32 *)0x4001000 &= ~0x40000000; resetBankForX_(data_0207cffe); }
