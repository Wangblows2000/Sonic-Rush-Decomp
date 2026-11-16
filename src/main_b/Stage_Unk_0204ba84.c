#include <nitro/types.h>

void NNS_SndCaptureStopReverb(s32 a);
void NNS_SndCaptureStopEffect(void);
void NNS_SndCaptureStopSampling(void);
void NNS_SndCaptureStopOutputEffect(void);
void func_ov000_022fabbc(void);
void Stage_Unk_0204ba84(void) { NNS_SndCaptureStopReverb(0); NNS_SndCaptureStopEffect(); NNS_SndCaptureStopSampling(); NNS_SndCaptureStopOutputEffect(); func_ov000_022fabbc(); }
