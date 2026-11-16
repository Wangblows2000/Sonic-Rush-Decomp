#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cfea[];
void GX_DisableBankForBG(void) { disableBankForX_(data_0207cfea); }
