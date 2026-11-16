#include <nitro/types.h>

extern u32 data_02250464;
extern u8 gBGMVolume;
void NNS_SndPlayerStopSeq(void *p, u32 a);
void NNS_SndHandleReleaseSeq(void *p);
void NNS_SndHandleInit(void *p);
void NNS_SndArcPlayerStartSeq(void *p, u32 a);
void NNS_SndPlayerSetVolume(void *p, u8 v);

void Sound_PlaySeq(u32 *p, u32 a) {
    if (p == NULL) p = &data_02250464;
    if (data_02250464 != 0) {
        NNS_SndPlayerStopSeq(&data_02250464, 0);
        NNS_SndHandleReleaseSeq(&data_02250464);
    }
    NNS_SndHandleInit(p);
    NNS_SndArcPlayerStartSeq(p, a);
    NNS_SndPlayerSetVolume(p, gBGMVolume);
}
