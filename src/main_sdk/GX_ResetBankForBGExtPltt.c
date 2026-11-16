#include <nitro/types.h>

void resetBankForX_(void *p);
extern u8 data_0207cff6[];
void GX_ResetBankForBGExtPltt(void) { *(vu32 *)0x4000000 &= ~0x40000000; resetBankForX_(data_0207cff6); }
