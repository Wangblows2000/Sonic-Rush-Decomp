#include <nitro/types.h>

void resetBankForX_(void *p);
extern u8 data_0207cff2[];
void GX_ResetBankForTexPltt(void) { resetBankForX_(data_0207cff2); }
