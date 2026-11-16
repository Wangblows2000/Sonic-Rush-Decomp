#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cfee[];
void GX_DisableBankForARM7(void) { disableBankForX_(data_0207cfee); }
