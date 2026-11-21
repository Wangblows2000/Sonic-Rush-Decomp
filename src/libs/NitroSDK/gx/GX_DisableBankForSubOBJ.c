#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cffc[];
void GX_DisableBankForSubOBJ(void) { disableBankForX_(data_0207cffc); }
