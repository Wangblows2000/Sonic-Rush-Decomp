#include <nitro/types.h>

typedef struct {
    fx32 _00, _01, _02;
    fx32 _10, _11, _12;
    fx32 _20, _21, _22;
} UnkMtx33_02000f34;

#define M64(x, y, z, b0, b1, b2) \
    ((((s64)(x) * (b0) + (s64)(y) * (b1) + (s64)(z) * (b2)) >> 12))
#define M(x, y, z, b0, b1, b2) \
    (fx32)(((s64)(x) * (b0) + (s64)(y) * (b1) + (s64)(z) * (b2)) >> 12)

void func_02000f34(const UnkMtx33_02000f34 *a, const UnkMtx33_02000f34 *b, UnkMtx33_02000f34 *ab)
{
    UnkMtx33_02000f34 tmp;
    UnkMtx33_02000f34 *p;
    fx32 x, y, z;
    fx32 xx, yy, zz;

    if (ab == b) {
        p = &tmp;
    } else {
        p = ab;
    }

    x = a->_00;
    y = a->_01;
    z = a->_02;
    p->_00 = M(x, y, z, b->_00, b->_10, b->_20);
    p->_01 = M(x, y, z, b->_01, b->_11, b->_21);
    xx = b->_02;
    yy = b->_12;
    zz = b->_22;
    p->_02 = M(x, y, z, xx, yy, zz);

    x = a->_10;
    y = a->_11;
    z = a->_12;
    p->_12 = M(x, y, z, xx, yy, zz);
    p->_11 = M(x, y, z, b->_01, b->_11, b->_21);
    xx = b->_00;
    yy = b->_10;
    zz = b->_20;
    p->_10 = M(x, y, z, xx, yy, zz);

    x = a->_20;
    y = a->_21;
    z = a->_22;
    p->_20 = M(x, y, z, xx, yy, zz);
    p->_21 = M(x, y, z, b->_01, b->_11, b->_21);
    xx = b->_02;
    yy = b->_12;
    zz = b->_22;
    p->_22 = M(x, y, z, xx, yy, zz);

    if (p == &tmp) {
        *ab = tmp;
    }
}
