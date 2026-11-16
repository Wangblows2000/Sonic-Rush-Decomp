#include <nitro/types.h>

void NNS_SndStopSoundAll(void);
void Sound_ResetHeap(void);

#pragma thumb on
void func_ov001_022d4500(void) {
    NNS_SndStopSoundAll();
    Sound_ResetHeap();
}
#pragma thumb reset
