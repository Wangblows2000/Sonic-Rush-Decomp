#include <nitro/types.h>

s32 SND_RecvCommandReply(u32 a);
void NNSi_SndPlayerMain(void);
void NNSi_SndCaptureMain(void);
void func_02020080(void);
void SND_FlushCommand(u32 a);
void NNS_SndMain(void) { while (SND_RecvCommandReply(0) != 0) {} NNSi_SndPlayerMain(); NNSi_SndCaptureMain(); func_02020080(); SND_FlushCommand(0); }
