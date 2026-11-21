#include <nitro/types.h>

void SNDi_SetTrackParam(u32 a, u32 b, u32 c, u32 d, u32 e);
void SND_SetTrackPitch(u32 a, u32 b, u32 d) { SNDi_SetTrackParam(a, b, 0xc, d, 2); }
