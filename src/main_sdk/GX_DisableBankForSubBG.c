#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cffa[];
void GX_DisableBankForSubBG(void) { disableBankForX_(data_0207cffa); }
