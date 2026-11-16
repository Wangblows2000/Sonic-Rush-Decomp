#include <nitro/types.h>

void OS_UnlockCard(u32 id);
void CARDi_UnlockResource(u32 a, u32 b);
void CARD_UnlockRom(u32 a) { OS_UnlockCard(a); CARDi_UnlockResource(a, 1); }
