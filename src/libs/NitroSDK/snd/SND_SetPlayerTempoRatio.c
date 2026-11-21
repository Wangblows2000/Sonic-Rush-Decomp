#include <nitro/types.h>

void SNDi_SetPlayerParam(u32 a, u32 off, u32 v, u32 sz);
void SND_SetPlayerTempoRatio(u32 a, u32 v) { SNDi_SetPlayerParam(a, 0x1a, v, 2); }
