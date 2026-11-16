#include <nitro/types.h>

typedef struct {
    s32 m[4];
} UnkMtx22_02056b20;

extern s16 data_0206e470[];
extern u8 data_02089264[];

void MTX_Rot22_(UnkMtx22_02056b20 *m, s32 s, s32 c);
void MTX_ScaleApply22(UnkMtx22_02056b20 *d, UnkMtx22_02056b20 *s, s32 x, s32 y);
void G2x_SetBGyAffine_(void *p, UnkMtx22_02056b20 *m, s32 a, s32 b, s32 x, s32 y);

#pragma thumb on
void func_02056b20(s32 x, s32 y) {
    UnkMtx22_02056b20 m = {0, 0, 0, 0};

    MTX_Rot22_(&m, data_0206e470[0], data_0206e470[1]);
    MTX_ScaleApply22(&m, &m, 0x1000, 0x1000);
    G2x_SetBGyAffine_(data_02089264, &m, 0, 0, -x, -y);
}
#pragma thumb reset
