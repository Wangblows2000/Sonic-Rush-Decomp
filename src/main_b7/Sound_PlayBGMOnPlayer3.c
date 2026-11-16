#include <nitro/types.h>

extern void *gBGMSoundHandle;
extern u8 gBGMVolume;
void NNS_SndPlayerStopSeq(void *a, s32 b);
void NNS_SndHandleReleaseSeq(void *a);
void NNS_SndHandleInit(void *a);
void NNS_SndArcPlayerStartSeqEx(void *a, s32 b, s32 c, s32 d, s32 e);
void NNS_SndPlayerSetVolume(void *a, s32 v);

void Sound_PlayBGMOnPlayer3(void **a, s32 b) {
    if (a == 0)
        a = &gBGMSoundHandle;
    if (gBGMSoundHandle != 0) {
        NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0);
        NNS_SndHandleReleaseSeq(&gBGMSoundHandle);
    }
    NNS_SndHandleInit(a);
    NNS_SndArcPlayerStartSeqEx(a, 3, -1, -1, b);
    NNS_SndPlayerSetVolume(a, gBGMVolume);
}
