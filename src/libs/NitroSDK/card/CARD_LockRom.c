#include <nitro/types.h>

void CARDi_LockResource(u32 a, u32 b);
void OS_LockCard(u32 id);
void CARD_LockRom(u32 a) { CARDi_LockResource(a, 1); OS_LockCard(a); }
