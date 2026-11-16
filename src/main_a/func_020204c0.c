#include <nitro/types.h>

extern u32 data_020779ec;
void SNDi_SetSurroundDecay(u32 a);
void NNS_SndCaptureStopEffect(void);

void func_020204c0(void) {
    if (data_020779ec == 1) {
        SNDi_SetSurroundDecay(0);
    }
    NNS_SndCaptureStopEffect();
}
