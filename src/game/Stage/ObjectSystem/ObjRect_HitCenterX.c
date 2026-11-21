#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s16 z;
    u16 flags;
    s16 l;
    s16 t;
    s16 r;
    s16 b;
} UnkCol_0206d5d0;

s32 ObjRect_HitCenterX(UnkCol_0206d5d0 *a, UnkCol_0206d5d0 *b) {
    s32 v[4];
    u8 i;
    s32 m;
    u8 k;
    u8 lo;
    u8 hi;
    s32 mn;
    s32 d;

    v[0] = a->x + a->l;
    v[1] = a->x + a->r;
    v[2] = b->x + b->l;
    v[3] = b->x + b->r;
    m = 0;
    k = 0;
    hi = 0;
    if (v[1] > m) {
        m = v[1];
        hi = 1;
    }
    if (v[2] > m) {
        m = v[2];
        hi = 2;
    }
    if (v[3] > m)
        hi = 3;
    mn = v[0];
    lo = 0;
    if (v[1] < mn) {
        mn = v[1];
        lo = 1;
    }
    if (v[2] < mn) {
        mn = v[2];
        lo = 2;
    }
    if (v[3] < mn)
        lo = 3;
    for (i = 0;; i++) {
        if (i != hi && i != lo) {
            v[k] = v[i];
            if (k != 0)
                break;
            k++;
        }
    }
    d = (v[0] - v[1]) >> 1;
    if (d < 0)
        d = -d;
    if (v[0] > v[1])
        m = d + v[1];
    else
        m = d + v[0];
    return m << 8;
}
