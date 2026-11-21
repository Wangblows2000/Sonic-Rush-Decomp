#include <nitro/types.h>

void NNS_SndPlayerStopSeqAll(u32 a); void NNS_SndArcStrmStopAll(u32 a); void NNSi_SndCaptureStop(void); void SNDi_SetSurroundDecay(u32 a);
void SND_StopTimer(u32 a, u32 b, u32 c, u32 d);
u32 SND_GetCurrentCommandTag(void); void SND_FlushCommand(u32 a); void SND_WaitForCommandProc(u32 a);
void NNS_SndStopSoundAll(void) {
    u32 r;
    NNS_SndPlayerStopSeqAll(0); NNS_SndArcStrmStopAll(0); NNSi_SndCaptureStop(); SNDi_SetSurroundDecay(0);
    SND_StopTimer(0xffff, 0xffff, 0xffff, 0);
    r = SND_GetCurrentCommandTag(); SND_FlushCommand(1); SND_WaitForCommandProc(r);
}
