#include <nitro/types.h>

void CARDi_WaitAsync(void);
void CARD_WaitRomAsync(void) { CARDi_WaitAsync(); }
