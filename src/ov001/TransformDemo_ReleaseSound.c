#include <nitro/types.h>

void NNS_SndStopSoundAll(void);
void NNS_SndHandleReleaseSeq(void *p);
void Sound_ResetHeap(void);

void TransformDemo_ReleaseSound(void *a, u8 *b) {
    NNS_SndStopSoundAll();
    NNS_SndHandleReleaseSeq(b + 4);
    Sound_ResetHeap();
}
