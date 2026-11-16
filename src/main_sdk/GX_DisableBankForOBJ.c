#include <nitro/types.h>

void disableBankForX_(void *p);
extern u8 data_0207cfec[];
void GX_DisableBankForOBJ(void) { disableBankForX_(data_0207cfec); }
