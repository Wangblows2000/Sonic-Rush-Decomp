#include <nitro/types.h>

typedef struct { u8 unk0[0xFC]; u32 unkFC; } UnkStruct_020220c0;
extern UnkStruct_020220c0 NNS_G3dGlb;
extern u8 data_02084e98[];
void calcSrtCameraMtx_(void);

void *NNS_G3dGlbGetInvWV(void) {
    if (!(NNS_G3dGlb.unkFC & 0x80)) {
        calcSrtCameraMtx_();
        NNS_G3dGlb.unkFC |= 0x80;
    }
    return data_02084e98;
}
