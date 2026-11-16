#include <nitro/types.h>

extern void *data_02250464;
extern u8 gBGMVolume;
void NNS_SndPlayerStopSeq(void *a, s32 b);
void NNS_SndHandleReleaseSeq(void *a);
void NNS_SndHandleInit(void *a);
void NNS_SndArcPlayerStartSeqEx(void *a, s32 b, s32 c, s32 d, s32 e);
void NNS_SndPlayerSetVolume(void *a, s32 v);

void func_0204b8e0(void **a, s32 b) {
    if (a == 0)
        a = &data_02250464;
    if (data_02250464 != 0) {
        NNS_SndPlayerStopSeq(&data_02250464, 0);
        NNS_SndHandleReleaseSeq(&data_02250464);
    }
    NNS_SndHandleInit(a);
    NNS_SndArcPlayerStartSeqEx(a, 3, -1, -1, b);
    NNS_SndPlayerSetVolume(a, gBGMVolume);
}
