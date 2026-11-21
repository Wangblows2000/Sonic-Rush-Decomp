#include <nitro/types.h>

s32 SND_RecvCommandReply(u32 a);
void NNSi_SndPlayerMain(void);
void NNSi_SndCaptureMain(void);
void NNSi_SndArcStrmMain(void);
void SND_FlushCommand(u32 a);
void NNS_SndMain(void) { while (SND_RecvCommandReply(0) != 0) {} NNSi_SndPlayerMain(); NNSi_SndCaptureMain(); NNSi_SndArcStrmMain(); SND_FlushCommand(0); }
