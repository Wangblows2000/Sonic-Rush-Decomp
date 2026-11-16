#include <nitro/types.h>

extern u32 CARDi_EnableFlag;
void CARD_Enable(u32 v) { CARDi_EnableFlag = v; }
