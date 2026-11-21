#include <nitro/types.h>

void NNSi_SndCaptureBeginSleep(void);
void SND_StopTimer(u32 a, u32 b, u32 c, u32 d);
u32 SND_GetCurrentCommandTag(void);
void SND_FlushCommand(u32 a);
void SND_WaitForCommandProc(u32 a);
void BeginSleep_nns_snd_main(void) { u32 r; NNSi_SndCaptureBeginSleep(); SND_StopTimer(0, 0, 0, 0); r = SND_GetCurrentCommandTag(); SND_FlushCommand(1); SND_WaitForCommandProc(r); }
