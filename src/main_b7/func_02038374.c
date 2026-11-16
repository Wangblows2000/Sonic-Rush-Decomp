#include <nitro/types.h>

typedef struct {
    fx32 x, y, z;
} VecFx32;

void func_0203841c(const VecFx32 *a, const VecFx32 *b, VecFx32 *out);
void VEC_Subtract(const VecFx32 *a, const VecFx32 *b, VecFx32 *out);
fx32 func_020026f8(const VecFx32 *a, const VecFx32 *b);

BOOL func_02038374(const VecFx32 *a, const VecFx32 *p0, const VecFx32 *p1, fx32 r, const VecFx32 *c) {
    VecFx32 n;
    VecFx32 d0;
    VecFx32 d1;
    func_0203841c(a, c, &n);
    VEC_Subtract(p0, &n, &d0);
    VEC_Subtract(p1, &n, &d1);
    VEC_Subtract(&n, c, &n);
    if ((s64)r * r < (s64)n.x * n.x + (s64)n.y * n.y + (s64)n.z * n.z)
        return FALSE;
    return func_020026f8(&d0, &d1) <= 0;
}
