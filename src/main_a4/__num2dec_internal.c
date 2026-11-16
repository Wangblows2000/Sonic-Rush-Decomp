#include <nitro/types.h>

#define HI(x) (*(1 + (s32 *)&(x)))
#define LO(x) (*(s32 *)&(x))

typedef struct {
    u8 sign;
    u8 unused;
    s16 exp;
    struct {
        u8 length;
        u8 text[32];
        u8 unused;
    } sig;
} UnkDecimal_0202cac4;

/* the routine at 0x0202de18 is named _ddiv in symbols.txt but computes a - b (_dsub) */
double _ddiv(double a, double b);
double frexp(double x, s32 *exp);
s32 MSL_Unk_0202d55c(u32 x);
void __two_exp(UnkDecimal_0202cac4 *d, s32 exp);
double ldexp(double x, int n);
u64 _ll_ufrom_d(double x);
void __ull2dec(UnkDecimal_0202cac4 *d, u64 x);
void __timesdec(UnkDecimal_0202cac4 *d, const UnkDecimal_0202cac4 *a, const UnkDecimal_0202cac4 *b);

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

static inline s32 CountTrailingZero(double x)
{
    u32 *l = (u32 *)&x;

    if (l[0] != 0)
        return MSL_Unk_0202d55c(l[0]);
    return MSL_Unk_0202d55c(l[1] | 0x00100000) + 32;
}

void __num2dec_internal(UnkDecimal_0202cac4 *d, double x)
{
    u8 sign = (u8)((HI(x) & 0x80000000) != 0);

    if (0.0 == x) {
        d->sign = sign;
        d->exp = 0;
        d->sig.length = 1;
        d->sig.text[0] = 0;
        return;
    }
    if (!(FpClassify(x) > 2)) {
        d->sign = sign;
        d->exp = 0;
        d->sig.length = 1;
        d->sig.text[0] = (FpClassify(x) == 1) ? 'N' : 'I';
        return;
    }
    if (sign)
        x = _ddiv(0.0, x);
    {
        s32 exp;
        double frac = frexp(x, &exp);
        s32 bits = 53 - CountTrailingZero(frac);
        UnkDecimal_0202cac4 intD, pow2D;

        __two_exp(&pow2D, exp - bits);
        __ull2dec(&intD, _ll_ufrom_d(ldexp(frac, bits)));
        __timesdec(d, &intD, &pow2D);
        d->sign = sign;
    }
}
