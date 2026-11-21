#include <nitro/types.h>

void resetBankForX_(void *p);
extern u8 data_0207cffc[];
void GX_ResetBankForSubOBJ(void) { resetBankForX_(data_0207cffc); }
