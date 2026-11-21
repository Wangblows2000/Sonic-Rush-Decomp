#include <nitro/types.h>

extern u32 gVoiceSoundHandle;
extern u8 gVoiceVolume;
void NNS_SndHandleReleaseSeq(void *p);
void NNS_SndHandleInit(void *p);
void NNS_SndArcPlayerStartSeqArc(void *p, u32 a, u32 b);
void NNS_SndPlayerSetVolume(void *p, u8 v);

void Sound_PlayVoice(u32 *p, u32 a, u32 b) {
    if (p == NULL) p = &gVoiceSoundHandle;
    if (gVoiceSoundHandle != 0) NNS_SndHandleReleaseSeq(&gVoiceSoundHandle);
    NNS_SndHandleInit(p);
    NNS_SndArcPlayerStartSeqArc(p, a, b);
    NNS_SndPlayerSetVolume(p, gVoiceVolume);
}
