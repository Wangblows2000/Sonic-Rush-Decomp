#include <nitro/types.h>

typedef struct {
    u16 angle;
    u16 pad2;
    s32 n;
    s32 f;
    s32 aspect;
    s32 scale;
    s32 pos[3];
    s32 target[3];
    s32 up[3];
    s32 tan;
    s32 dist;
} UnkCam_022dfc54;

typedef struct {
    u8 pad0[0x10];
    s32 mode;
} UnkWork_022dfc54;

typedef struct {
    u8 pad0[0xfc];
    u32 flags;
} UnkG3_022dfc54;

extern s16 data_0206e470[];
extern u8 data_02084d40[];
extern UnkG3_022dfc54 NNS_G3dGlb;

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void MIi_CpuClear16(u16 v, void *dst, u32 size);
void G3i_PerspectiveW_(s32 s, s32 c, s32 aspect, s32 n, s32 f, s32 scale, s32 z, void *mtx);
s32 FX_Div(s32 a, s32 b);

static inline void MI_CpuClear16_dfc54(void *dst, u32 size) {
    MIi_CpuClear16(0, dst, size);
}

static inline s32 Tan_dfc54(u16 ang) {
    return FX_Div(SIN_IDX(ang >> 4), COS_IDX(ang >> 4));
}

void TransformDemo_InitCamera(UnkWork_022dfc54 *w, UnkCam_022dfc54 *c) {
    s32 idx;
    s32 a;
    u16 b;
    vu16 zero = 0;

    MIi_CpuClear16(zero, c, 0x14);
    c->angle = 0x888;
    c->n = 0x1000;
    c->f = 0x800000;
    c->aspect = 0x1555;
    c->scale = 0x1000;
    idx = (s32)(c->angle & 0xffff) >> 4;
    G3i_PerspectiveW_(SIN_IDX(idx), COS_IDX(idx), c->aspect, c->n, c->f, c->scale, 0, data_02084d40);
    NNS_G3dGlb.flags &= ~0x50;
    c->pos[0] = 0;
    c->pos[1] = 0;
    c->pos[2] = 0x19000;
    c->up[0] = 0;
    c->up[1] = 0x1000;
    c->up[2] = 0;
    c->target[0] = 0;
    c->target[1] = 0;
    c->target[2] = 0;
    if (w->mode != 2)
        a = 0x71c;
    else
        a = 0x4fa;
    b = a;
    c->tan = Tan_dfc54((u16)(0x1110 + b));
    c->dist = (s32)(((s64)FX_Div(SIN_IDX((u16)(0x888 + ((u32)b >> 1)) >> 4), COS_IDX(((u16)(0x888 + ((u32)b >> 1)) & 0xffff) >> 4)) * 0x19000 + 0x800) >> 12);
}
