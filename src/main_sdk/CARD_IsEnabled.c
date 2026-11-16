#include <nitro/types.h>

extern u32 CARDi_EnableFlag;
u32 CARD_IsEnabled(void) { return CARDi_EnableFlag; }
