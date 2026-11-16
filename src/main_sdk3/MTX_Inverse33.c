#include <nitro/types.h>

typedef struct {
    fx32 _[9];
} UnkMtx33_020010d0;

void FX_InvAsync(fx32 x);
fx32 FX_GetDivResult(void);
void MI_Copy36B(const void *src, void *dst);

s32 MTX_Inverse33(const UnkMtx33_020010d0 *src, UnkMtx33_020010d0 *dst)
{
    UnkMtx33_020010d0 tmp;
    UnkMtx33_020010d0 *p;
    fx32 det;
    fx32 det0, det1, det2;
    fx32 var0, var1, var2, var3;

    if (src == dst) {
        p = &tmp;
    } else {
        p = dst;
    }

    det0 = (fx32)(((s64)src->_[4] * src->_[8] - (s64)src->_[5] * src->_[7] + 0x800) >> 12);
    det1 = (fx32)(((s64)src->_[3] * src->_[8] - (s64)src->_[5] * src->_[6] + 0x800) >> 12);
    det2 = (fx32)(((s64)src->_[3] * src->_[7] - (s64)src->_[4] * src->_[6] + 0x800) >> 12);

    det = (fx32)(((s64)src->_[0] * det0 - (s64)src->_[1] * det1 + (s64)src->_[2] * det2 + 0x800) >> 12);

    if (det == 0) {
        return -1;
    }

    FX_InvAsync(det);

    var0 = (fx32)(((s64)src->_[1] * src->_[8] - (s64)src->_[7] * src->_[2]) >> 12);
    var1 = (fx32)(((s64)src->_[1] * src->_[5] - (s64)src->_[4] * src->_[2]) >> 12);
    var2 = (fx32)(((s64)src->_[0] * src->_[8] - (s64)src->_[6] * src->_[2]) >> 12);
    var3 = (fx32)(((s64)src->_[0] * src->_[5] - (s64)src->_[3] * src->_[2]) >> 12);

    det = FX_GetDivResult();

    p->_[0] = (fx32)(((s64)det * det0) >> 12);
    p->_[1] = -(fx32)(((s64)det * var0) >> 12);
    p->_[2] = (fx32)(((s64)det * var1) >> 12);

    p->_[3] = -(fx32)(((s64)det * det1) >> 12);
    p->_[4] = (fx32)(((s64)det * var2) >> 12);
    p->_[5] = -(fx32)(((s64)det * var3) >> 12);

    p->_[6] = (fx32)(((s64)det * det2) >> 12);
    p->_[7] = -(fx32)(((s64)det * (fx32)(((s64)src->_[0] * src->_[7] - (s64)src->_[6] * src->_[1]) >> 12)) >> 12);
    p->_[8] = (fx32)(((s64)det * (fx32)(((s64)src->_[0] * src->_[4] - (s64)src->_[3] * src->_[1]) >> 12)) >> 12);


    if (p == &tmp) {
        MI_Copy36B(&tmp, dst);
    }
    return 0;
}
