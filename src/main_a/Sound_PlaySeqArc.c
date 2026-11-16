#include <nitro/types.h>

extern u32 gSESoundHandle;
extern u8 gSEVolume;
void NNS_SndHandleReleaseSeq(void *p);
void NNS_SndHandleInit(void *p);
void NNS_SndArcPlayerStartSeqArc(void *p, u32 a, u32 b);
void NNS_SndPlayerSetVolume(void *p, u8 v);

void Sound_PlaySeqArc(u32 *p, u32 a, u32 b) {
    if (p == NULL) p = &gSESoundHandle;
    if (gSESoundHandle != 0) NNS_SndHandleReleaseSeq(&gSESoundHandle);
    NNS_SndHandleInit(p);
    NNS_SndArcPlayerStartSeqArc(p, a, b);
    NNS_SndPlayerSetVolume(p, gSEVolume);
}
