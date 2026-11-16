#include <nitro/types.h>

typedef struct {
    fx32 x;
    fx32 y;
} UnkPt_022e1e98;

fx32 FX_Sqrt(fx32 a);
fx32 func_0200225c(fx32 a, fx32 b);
void func_ov001_022e1ce8(s32 n, fx32 *t, fx32 *v, fx32 *c);
fx32 func_ov001_022e1bec(fx32 s, s32 n, fx32 *t, fx32 *v, fx32 *c);

static inline fx32 FX_Mul_022e1e98(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

#pragma thumb on
void func_ov001_022e1e98(s32 n, s32 *out, fx32 x0, fx32 y0, fx32 x1, fx32 y1, fx32 x2, fx32 y2, fx32 x3, fx32 y3) {
    s32 i;
    fx32 dx;
    fx32 dy;
    fx32 s;
    fx32 t[4];
    fx32 cx[4];
    fx32 cy[4];
    fx32 xs[4];
    fx32 ys[4];

    xs[0] = x0;
    xs[1] = x1;
    xs[2] = x2;
    xs[3] = x3;
    ys[0] = y0;
    ys[1] = y1;
    ys[2] = y2;
    ys[3] = y3;
    t[0] = 0;
    for (i = 1; i < 4; i++) {
        dx = xs[i] - xs[i - 1];
        dy = ys[i] - ys[i - 1];
        t[i] = t[i - 1] + FX_Sqrt(FX_Mul_022e1e98(dx, dx) + FX_Mul_022e1e98(dy, dy));
    }
    for (i = 1; i < 4; i++) {
        t[i] = func_0200225c(t[i], t[3]);
    }
    func_ov001_022e1ce8(4, t, xs, cx);
    func_ov001_022e1ce8(4, t, ys, cy);
    for (i = 0; i < n; i++) {
        s = func_0200225c((i + 1) << 12, (n + 1) << 12);
        out[0] = func_ov001_022e1bec(s, 4, t, xs, cx) >> 12;
        out[1] = func_ov001_022e1bec(s, 4, t, ys, cy) >> 12;
        out += 2;
    }
}
#pragma thumb reset
