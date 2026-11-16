#include <nitro/types.h>

void NNS_SndStopSoundAll(void);
void NNS_SndHandleReleaseSeq(void *p);
void Sound_ResetHeap(void);

void func_ov001_022ddfa4(void *a, u8 *b) {
    NNS_SndStopSoundAll();
    NNS_SndHandleReleaseSeq(b + 4);
    Sound_ResetHeap();
}
