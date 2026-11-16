#include <nitro/types.h>

#define HI(x) (*(1 + (s32 *)&(x)))
#define LO(x) (*(s32 *)&(x))

double func_0202c60c(double x, double y);

static inline int FpClassify(double x)
{
    switch (HI(x) & 0x7ff00000) {
    case 0x7ff00000:
        if ((HI(x) & 0x000fffff) || LO(x))
            return 1;
        else
            return 2;
    case 0:
        if ((HI(x) & 0x000fffff) || LO(x))
            return 5;
        else
            return 3;
    }
    return 4;
}

double func_0202c718(double x, int n)
{
    s32 k, hx, lx;

    if (!(FpClassify(x) > 2) || 0.0 == x)
        return x;
    hx = HI(x);
    lx = LO(x);
    k = (hx & 0x7ff00000) >> 20;
    if (k == 0) {
        if ((lx | (hx & 0x7fffffff)) == 0)
            return x;
        x *= 1.80143985094819840000e+16;
        hx = HI(x);
        k = ((hx & 0x7ff00000) >> 20) - 54;
        if (n < -50000)
            return 1.0e-300 * x;
    }
    if (k == 0x7ff)
        return x + x;
    k = k + n;
    if (k > 0x7fe)
        return 1.0e+300 * func_0202c60c(1.0e+300, x);
    if (k > 0) {
        HI(x) = (hx & 0x800fffff) | (k << 20);
        return x;
    }
    if (k <= -54) {
        if (n > 50000)
            return 1.0e+300 * func_0202c60c(1.0e+300, x);
        else
            return 1.0e-300 * func_0202c60c(1.0e-300, x);
    }
    k += 54;
    HI(x) = (hx & 0x800fffff) | (k << 20);
    return 5.55111512312578270212e-17 * x;
}
