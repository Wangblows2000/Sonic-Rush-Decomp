#include <nitro/types.h>

typedef struct {
    fx32 _00, _01, _02;
    fx32 _10, _11, _12;
    fx32 _20, _21, _22;
    fx32 _30, _31, _32;
} UnkMtx43_0200160c;

#define M64(x, y, z, b0, b1, b2) \
    ((((s64)(x) * (b0) + (s64)(y) * (b1) + (s64)(z) * (b2)) >> 12))
#define M(x, y, z, b0, b1, b2) \
    (fx32)(((s64)(x) * (b0) + (s64)(y) * (b1) + (s64)(z) * (b2)) >> 12)

void func_0200160c(const UnkMtx43_0200160c *a, const UnkMtx43_0200160c *b, UnkMtx43_0200160c *ab)
{
    UnkMtx43_0200160c tmp;
    UnkMtx43_0200160c *p;
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

    x = a->_30;
    y = a->_31;
    z = a->_32;
    p->_32 = (fx32)(b->_32 + M64(x, y, z, xx, yy, zz));
    p->_31 = (fx32)(b->_31 + M64(x, y, z, b->_01, b->_11, b->_21));
    p->_30 = (fx32)(b->_30 + M64(x, y, z, b->_00, b->_10, b->_20));

    if (p == &tmp) {
        *ab = tmp;
    }
}
