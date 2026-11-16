#include <nitro/types.h>

extern u32 data_02250450;
extern u8 data_02250448;
void NNS_SndHandleReleaseSeq(void *p);
void NNS_SndHandleInit(void *p);
void NNS_SndArcPlayerStartSeqArc(void *p, u32 a, u32 b);
void NNS_SndPlayerSetVolume(void *p, u8 v);

void func_020366f4(u32 *p, u32 a, u32 b) {
    if (p == NULL) p = &data_02250450;
    if (data_02250450 != 0) NNS_SndHandleReleaseSeq(&data_02250450);
    NNS_SndHandleInit(p);
    NNS_SndArcPlayerStartSeqArc(p, a, b);
    NNS_SndPlayerSetVolume(p, data_02250448);
}
