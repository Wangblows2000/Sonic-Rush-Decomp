#include <nitro/types.h>

typedef struct { s16 x, y, z; } UnkStruct_022df878;
extern u16 data_ov001_023161fe[];
extern u16 data_ov001_02316204[];
extern UnkStruct_022df878 data_ov001_0231621e[];
extern UnkStruct_022df878 data_ov001_02316230[];
void NNS_G3dGlbLightColor(s32 i, u16 v);
void NNS_G3dGlbLightVector(s32 i, s32 x, s32 y, s32 z);

void TransformDemo_UpdateLightFlash(u8 *p) {
    u16 *a;
    UnkStruct_022df878 *c;
    u8 i;
    if (*(s32 *)(p + 0x10) != 2) {
        return;
    }
    *(s8 *)(p + 0xeb0) += 1;
    if (*(s8 *)(p + 0xeb0) < 6) {
        return;
    }
    *(s8 *)(p + 0xeb0) = 0;
    *(s8 *)(p + 0xeb1) = *(s8 *)(p + 0xeb1) == 0 ? 1 : 0;
    if (*(s8 *)(p + 0xeb1) != 0) {
        a = data_ov001_023161fe;
        c = data_ov001_0231621e;
    } else {
        a = data_ov001_02316204;
        c = data_ov001_02316230;
    }
    NNS_G3dGlbLightColor(0, a[0]);
    NNS_G3dGlbLightColor(1, a[1]);
    NNS_G3dGlbLightColor(2, a[2]);
    for (i = 0; i < 3; i++) {
        NNS_G3dGlbLightVector(i, c[i].x, c[i].y, c[i].z);
    }
}
