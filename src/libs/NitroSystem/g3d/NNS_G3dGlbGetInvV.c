#include <nitro/types.h>

typedef struct { u8 unk0[0xFC]; u32 unkFC; } UnkStruct_020221ac;
extern UnkStruct_020221ac NNS_G3dGlb;
extern u8 data_02084d84[];
extern u8 data_02084e38[];
void MTX_Inverse43(void *a, void *b);

void *NNS_G3dGlbGetInvV(void) {
    if (!(NNS_G3dGlb.unkFC & 8)) {
        MTX_Inverse43(data_02084d84, data_02084e38);
        NNS_G3dGlb.unkFC |= 8;
    }
    return data_02084e38;
}
