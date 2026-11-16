#include <nitro/types.h>

double _dmul(double a, double b);

double func_0202c658(double x, s32 *eptr) {
    s32 hx = *(1 + (s32 *)&x);
    s32 ix = hx & 0x7FFFFFFF;
    s32 lx = *(s32 *)&x;
    *eptr = 0;
    if (ix >= 0x7FF00000 || (ix | lx) == 0) return x;
    if (ix < 0x00100000) {
        x = _dmul(x, 18014398509481984.0);
        hx = *(1 + (s32 *)&x);
        ix = hx & 0x7FFFFFFF;
        *eptr = -54;
    }
    *eptr += (ix >> 20) - 1022;
    hx = (hx & 0x800FFFFF) | 0x3FE00000;
    *(1 + (s32 *)&x) = hx;
    return x;
}
