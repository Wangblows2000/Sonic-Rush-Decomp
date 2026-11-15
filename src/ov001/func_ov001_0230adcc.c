#include <nitro/types.h>

typedef struct { s32 x, y, z; } VecFx32;
s32 func_02002278(VecFx32 *a, VecFx32 *b);

BOOL func_ov001_0230adcc(s32 *a, s32 *b, s32 x, s32 y) {
    VecFx32 p, q;
    p.x = b[0] + a[0];
    p.y = b[1] + a[1];
    p.z = 0;
    q.x = x;
    q.y = y;
    q.z = 0;
    return func_02002278(&p, &q) < a[2];
}
