#include <nitro/types.h>

extern u32 gBGMSoundHandle;
extern u8 gBGMVolume;
void NNS_SndPlayerStopSeq(void *p, u32 a);
void NNS_SndHandleReleaseSeq(void *p);
void NNS_SndHandleInit(void *p);
void NNS_SndArcPlayerStartSeq(void *p, u32 a);
void NNS_SndPlayerSetVolume(void *p, u8 v);

void Sound_PlaySeq(u32 *p, u32 a) {
    if (p == NULL) p = &gBGMSoundHandle;
    if (gBGMSoundHandle != 0) {
        NNS_SndPlayerStopSeq(&gBGMSoundHandle, 0);
        NNS_SndHandleReleaseSeq(&gBGMSoundHandle);
    }
    NNS_SndHandleInit(p);
    NNS_SndArcPlayerStartSeq(p, a);
    NNS_SndPlayerSetVolume(p, gBGMVolume);
}
