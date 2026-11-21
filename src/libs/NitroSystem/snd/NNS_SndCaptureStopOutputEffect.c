#include <nitro/types.h>

extern u32 data_020779ec;
void SNDi_SetSurroundDecay(u32 a);
void NNS_SndCaptureStopEffect(void);

void NNS_SndCaptureStopOutputEffect(void) {
    if (data_020779ec == 1) {
        SNDi_SetSurroundDecay(0);
    }
    NNS_SndCaptureStopEffect();
}
