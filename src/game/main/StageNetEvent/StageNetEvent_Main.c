#include <nitro/types.h>

void NNS_SndCaptureStopReverb(s32 a);
void NNS_SndCaptureStopEffect(void);
void NNS_SndCaptureStopSampling(void);
void NNS_SndCaptureStopOutputEffect(void);
void Stage_NetProcessRivalEvents(void);
void StageNetEvent_Main(void) { NNS_SndCaptureStopReverb(0); NNS_SndCaptureStopEffect(); NNS_SndCaptureStopSampling(); NNS_SndCaptureStopOutputEffect(); Stage_NetProcessRivalEvents(); }
