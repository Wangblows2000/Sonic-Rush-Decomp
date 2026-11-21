#include <nitro/types.h>

void resetBankForX_(void *p);
extern u8 data_0207cff0[];
void GX_ResetBankForTex(void) { resetBankForX_(data_0207cff0); }
