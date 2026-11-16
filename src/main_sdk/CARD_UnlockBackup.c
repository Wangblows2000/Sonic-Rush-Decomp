#include <nitro/types.h>

u32 CARDi_UnlockResource(u32 a, u32 b);
u32 CARD_UnlockBackup(u32 a) { return CARDi_UnlockResource(a, 2); }
