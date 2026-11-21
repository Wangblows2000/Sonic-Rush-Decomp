#include <nitro/types.h>

void SND_StopUnlockedChannel(u32 a, u32 b);
void NNS_SndStopChannelAll(void) { SND_StopUnlockedChannel(0xffff, 0); }
