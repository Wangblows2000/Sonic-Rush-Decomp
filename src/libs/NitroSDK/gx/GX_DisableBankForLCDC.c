#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cfe8[];
void GX_DisableBankForLCDC(void) { disableBankForX_(data_0207cfe8); }
