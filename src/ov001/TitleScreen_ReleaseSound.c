#include <nitro/types.h>

void NNS_SndStopSoundAll(void);
void Sound_ResetHeap(void);

#pragma thumb on
void TitleScreen_ReleaseSound(void) {
    NNS_SndStopSoundAll();
    Sound_ResetHeap();
}
#pragma thumb reset
