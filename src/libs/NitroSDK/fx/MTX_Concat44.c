#include <nitro/types.h>

typedef struct {
    fx32 _00, _01, _02, _03;
    fx32 _10, _11, _12, _13;
    fx32 _20, _21, _22, _23;
    fx32 _30, _31, _32, _33;
} UnkMtx44_02001d18;

#define M(x, y, z, w, b0, b1, b2, b3) \
    (fx32)(((s64)(x) * (b0) + (s64)(y) * (b1) + (s64)(z) * (b2) + (s64)(w) * (b3)) >> 12)

void MTX_Concat44(const UnkMtx44_02001d18 *a, const UnkMtx44_02001d18 *b, UnkMtx44_02001d18 *ab)
{
    UnkMtx44_02001d18 tmp;
    UnkMtx44_02001d18 *p;
    fx32 x, y, z, w;
    fx32 xx, yy, zz, ww;

    if (ab == b) {
        p = &tmp;
    } else {
        p = ab;
    }

    x = a->_00;
    y = a->_01;
    z = a->_02;
    w = a->_03;
    p->_00 = M(x, y, z, w, b->_00, b->_10, b->_20, b->_30);
    p->_01 = M(x, y, z, w, b->_01, b->_11, b->_21, b->_31);
    p->_03 = M(x, y, z, w, b->_03, b->_13, b->_23, b->_33);
    xx = b->_02;
    yy = b->_12;
    zz = b->_22;
    ww = b->_32;
    p->_02 = M(x, y, z, w, xx, yy, zz, ww);

    x = a->_10;
    y = a->_11;
    z = a->_12;
    w = a->_13;
    p->_12 = M(x, y, z, w, xx, yy, zz, ww);
    p->_11 = M(x, y, z, w, b->_01, b->_11, b->_21, b->_31);
    p->_13 = M(x, y, z, w, b->_03, b->_13, b->_23, b->_33);
    xx = b->_00;
    yy = b->_10;
    zz = b->_20;
    ww = b->_30;
    p->_10 = M(x, y, z, w, xx, yy, zz, ww);

    x = a->_20;
    y = a->_21;
    z = a->_22;
    w = a->_23;
    p->_20 = M(x, y, z, w, xx, yy, zz, ww);
    p->_21 = M(x, y, z, w, b->_01, b->_11, b->_21, b->_31);
    p->_23 = M(x, y, z, w, b->_03, b->_13, b->_23, b->_33);
    xx = b->_02;
    yy = b->_12;
    zz = b->_22;
    ww = b->_32;
    p->_22 = M(x, y, z, w, xx, yy, zz, ww);

    x = a->_30;
    y = a->_31;
    z = a->_32;
    w = a->_33;
    p->_32 = M(x, y, z, w, xx, yy, zz, ww);
    p->_31 = M(x, y, z, w, b->_01, b->_11, b->_21, b->_31);
    p->_30 = M(x, y, z, w, b->_00, b->_10, b->_20, b->_30);
    p->_33 = M(x, y, z, w, b->_03, b->_13, b->_23, b->_33);

    if (p == &tmp) {
        *ab = tmp;
    }
}
