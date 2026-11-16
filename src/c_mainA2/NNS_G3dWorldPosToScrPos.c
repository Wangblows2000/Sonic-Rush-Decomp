#include <nitro/types.h>

typedef struct {
    fx32 x, y, z;
} UnkVec_020254d4;

typedef struct {
    fx32 _00, _01, _02, _03;
    fx32 _10, _11, _12, _13;
    fx32 _20, _21, _22, _23;
    fx32 _30, _31, _32, _33;
} UnkMtx44_020254d4;

typedef struct {
    fx32 m[12];
} UnkMtx43_020254d4;

extern UnkMtx44_020254d4 data_02084d40;
extern UnkMtx43_020254d4 data_02084d84;

void MTX_MultVec43(const UnkVec_020254d4 *v, const UnkMtx43_020254d4 *m, UnkVec_020254d4 *dst);
void FX_InvAsync(fx32 x);
s64 FX_GetDivResultFx64c(void);
void NNS_G3dGlbGetViewPort(int *x1, int *y1, int *x2, int *y2);

static inline fx32 FX_Mul32x64c_020254d4(fx32 v, s64 c) {
    return (fx32)((c * v + 0x80000000LL) >> 32);
}

int NNS_G3dWorldPosToScrPos(const UnkVec_020254d4 *pWorld, int *px, int *py) {
    const UnkMtx44_020254d4 *proj;
    const UnkMtx43_020254d4 *camera;
    UnkVec_020254d4 tmp;
    UnkVec_020254d4 vec;
    fx32 w;
    s64 invW;
    int x1, y1, x2, y2;
    int dx, dy;
    int rval;

    proj = (const UnkMtx44_020254d4 *)(u32)&data_02084d40;
    camera = &data_02084d84;
    MTX_MultVec43(pWorld, camera, &tmp);
    w = (fx32)(((s64)tmp.x * proj->_03 + (s64)tmp.y * proj->_13 + (s64)tmp.z * proj->_23) >> 12);
    w += proj->_33;
    FX_InvAsync(w);
    vec.x = (fx32)(((s64)tmp.x * proj->_00 + (s64)tmp.y * proj->_10 + (s64)tmp.z * proj->_20) >> 12);
    vec.x += proj->_30;
    vec.y = (fx32)(((s64)tmp.x * proj->_01 + (s64)tmp.y * proj->_11 + (s64)tmp.z * proj->_21) >> 12);
    vec.y += proj->_31;
    invW = FX_GetDivResultFx64c();
    vec.x = (FX_Mul32x64c_020254d4(vec.x, invW) + 0x1000) / 2;
    vec.y = (FX_Mul32x64c_020254d4(vec.y, invW) + 0x1000) / 2;
    if (vec.x < 0 || vec.y < 0 || vec.x > 0x1000 || vec.y > 0x1000)
        rval = -1;
    else
        rval = 0;
    NNS_G3dGlbGetViewPort(&x1, &y1, &x2, &y2);
    dx = x2 - x1;
    dy = y2 - y1;
    *px = x1 + ((vec.x * dx + 0x800) >> 12);
    *py = 191 - y1 - ((vec.y * dy + 0x800) >> 12);
    return rval;
}
