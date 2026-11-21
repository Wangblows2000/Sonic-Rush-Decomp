#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cff0[];
void GX_DisableBankForTex(void) { disableBankForX_(data_0207cff0); }
