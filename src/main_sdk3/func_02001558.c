#include <nitro/types.h>

typedef struct {
    fx32 x, y, z;
} UnkVec_02001558;

typedef struct {
    fx32 _00, _01, _02;
    fx32 _10, _11, _12;
    fx32 _20, _21, _22;
    fx32 _30, _31, _32;
} UnkMtx43_02001558;

void func_02001558(const UnkVec_02001558 *vec, const UnkMtx43_02001558 *m, UnkVec_02001558 *dst)
{
    fx32 x = vec->x;
    fx32 y = vec->y;
    fx32 z = vec->z;

    dst->x = (fx32)(((s64)x * m->_00 + (s64)y * m->_10 + (s64)z * m->_20) >> 12);
    dst->x += m->_30;
    dst->y = (fx32)(((s64)x * m->_01 + (s64)y * m->_11 + (s64)z * m->_21) >> 12);
    dst->y += m->_31;
    dst->z = (fx32)(((s64)x * m->_02 + (s64)y * m->_12 + (s64)z * m->_22) >> 12);
    dst->z += m->_32;
}
