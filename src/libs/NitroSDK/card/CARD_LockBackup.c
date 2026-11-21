#include <nitro/types.h>

u32 CARDi_LockResource(u32 a, u32 b);
u32 CARD_LockBackup(u32 a) { return CARDi_LockResource(a, 2); }
