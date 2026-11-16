#include <nitro/types.h>

void SND_UnlockChannel(u32 a, u32 b);
extern u32 data_02083448;
void NNS_SndUnlockChannel(u32 m) { if (m == 0) return; SND_UnlockChannel(m, 0); data_02083448 &= ~m; }
