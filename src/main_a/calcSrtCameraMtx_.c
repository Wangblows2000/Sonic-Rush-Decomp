#include <nitro/types.h>

typedef struct { u8 unk0[0xEC]; u32 unkEC; u32 unkF0; u32 unkF4; } UnkStruct_02022148;
extern UnkStruct_02022148 NNS_G3dGlb;
extern u8 data_02084df4[];
extern u8 data_02084d84[];
extern u8 data_02084e68[];
extern u8 data_02084e98[];
void MTX_Concat43(void *a, void *b, void *c);
void MTX_ScaleApply43(void *a, void *b, u32 c, u32 d, u32 e);
void MTX_Inverse43(void *a, void *b);

void calcSrtCameraMtx_(void) {
    MTX_Concat43(data_02084df4, data_02084d84, data_02084e68);
    MTX_ScaleApply43(data_02084e68, data_02084e68, NNS_G3dGlb.unkEC, NNS_G3dGlb.unkF0, NNS_G3dGlb.unkF4);
    MTX_Inverse43(data_02084e68, data_02084e98);
}
