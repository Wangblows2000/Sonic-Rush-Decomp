#include <nitro/types.h>

typedef struct {
    s32 m[4];
} UnkMtx22_02061a08;

typedef struct {
    u8 pad0[0x24];
    s32 x;
    s32 y;
    s16 sx;
    s16 sy;
    u16 angle;
} UnkObj_02061a08;

extern s16 data_0206e470[];
extern u8 data_02089254[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void MTX_Rot22_(UnkMtx22_02061a08 *m, s32 s, s32 c);
void MTX_ScaleApply22(UnkMtx22_02061a08 *d, UnkMtx22_02061a08 *s, s32 x, s32 y);
void G2x_SetBGyAffine_(void *p, UnkMtx22_02061a08 *m, s32 a, s32 b, s32 x, s32 y);

#pragma thumb on
void TalkDemo_ApplyBg2Affine(UnkObj_02061a08 *o) {
    UnkMtx22_02061a08 m = {0, 0, 0, 0};
    s32 idx;

    idx = (s32)(o->angle & 0xffff) >> 4;
    MTX_Rot22_(&m, SIN_IDX(idx), COS_IDX(idx));
    MTX_ScaleApply22(&m, &m, o->sx << 4, o->sy << 4);
    G2x_SetBGyAffine_(data_02089254, &m, 0, 0, -(o->x >> 8), -(o->y >> 8));
}
#pragma thumb reset
