#include <nitro/types.h>

typedef struct {
    fx32 x, y, z;
} UnkVec_02000eb0;

typedef struct {
    fx32 _00, _01, _02;
    fx32 _10, _11, _12;
    fx32 _20, _21, _22;
} UnkMtx33_02000eb0;

void func_02000eb0(const UnkVec_02000eb0 *vec, const UnkMtx33_02000eb0 *m, UnkVec_02000eb0 *dst)
{
    fx32 x = vec->x;
    fx32 y = vec->y;
    fx32 z = vec->z;

    dst->x = (fx32)(((s64)x * m->_00 + (s64)y * m->_10 + (s64)z * m->_20) >> 12);
    dst->y = (fx32)(((s64)x * m->_01 + (s64)y * m->_11 + (s64)z * m->_21) >> 12);
    dst->z = (fx32)(((s64)x * m->_02 + (s64)y * m->_12 + (s64)z * m->_22) >> 12);
}
