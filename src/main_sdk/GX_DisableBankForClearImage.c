#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cff4[];
void GX_DisableBankForClearImage(void) { disableBankForX_(data_0207cff4); }
