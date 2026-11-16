#include <nitro/types.h>

typedef struct {
    fx32 _00, _01, _10, _11;
} UnkMtx22_02000ce4;

void MTX_Concat22(const UnkMtx22_02000ce4 *a, const UnkMtx22_02000ce4 *b, UnkMtx22_02000ce4 *ab)
{
    UnkMtx22_02000ce4 tmp;
    UnkMtx22_02000ce4 *p;
    fx32 x, y;

    if (ab == b) {
        p = &tmp;
    } else {
        p = ab;
    }
    x = a->_00;
    y = a->_01;
    p->_00 = (fx32)(((s64)x * b->_00 + (s64)y * b->_10) >> 12);
    p->_01 = (fx32)(((s64)x * b->_01 + (s64)y * b->_11) >> 12);
    x = a->_10;
    y = a->_11;
    p->_10 = (fx32)(((s64)x * b->_00 + (s64)y * b->_10) >> 12);
    p->_11 = (fx32)(((s64)x * b->_01 + (s64)y * b->_11) >> 12);
    if (p == &tmp) {
        *ab = tmp;
    }
}
