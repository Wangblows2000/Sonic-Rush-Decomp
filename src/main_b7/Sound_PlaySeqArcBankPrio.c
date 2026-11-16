#include <nitro/types.h>

extern void *gSESoundHandle;
extern u8 gSEVolume;
void NNS_SndHandleReleaseSeq(void *a);
void NNS_SndHandleInit(void *a);
void func_0201e9cc(void *a, s32 b, s32 c, s32 d, s32 e, s32 f);
void NNS_SndPlayerSetVolume(void *a, s32 v);

void Sound_PlaySeqArcBankPrio(void **a, s32 b, s32 c, s32 d, s32 e) {
    if (a == 0)
        a = &gSESoundHandle;
    if (gSESoundHandle != 0)
        NNS_SndHandleReleaseSeq(&gSESoundHandle);
    NNS_SndHandleInit(a);
    func_0201e9cc(a, -1, e, d, b, c);
    NNS_SndPlayerSetVolume(a, gSEVolume);
}
