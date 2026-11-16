#include <nitro/types.h>

void resetBankForX_(void *p);
extern u8 data_0207cffa[];
void GX_ResetBankForSubBG(void) { resetBankForX_(data_0207cffa); }
