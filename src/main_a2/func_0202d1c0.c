#include <nitro/types.h>

typedef struct {
    s8 sign;
    u8 unused;
    s16 exp;
    struct {
        u8 length;
        u8 text[32];
        u8 unused;
    } sig;
} UnkDecimal_0202d1c0;

void func_0202d49c(UnkDecimal_0202d1c0 *d, s32 n);

void func_0202d1c0(UnkDecimal_0202d1c0 *result, const UnkDecimal_0202d1c0 *x, const UnkDecimal_0202d1c0 *y) {
    u32 accum = 0;
    u8 mantissa[0x41];
    s32 i = x->sig.length + y->sig.length - 1;
    u8 *pDigit = mantissa + i + 1;
    u8 *end = pDigit;
    result->sign = 0;
    for (; i > 0; i--) {
        s32 k = y->sig.length - 1;
        s32 j = i - k - 1;
        s32 l;
        s32 t;
        const u8 *ip;
        const u8 *jp;
        if (j < 0) {
            j = 0;
            k = i - 1;
        }
        ip = x->sig.text + j;
        jp = y->sig.text + k;
        l = k + 1;
        t = x->sig.length - j;
        if (l > t) l = t;
        for (; l > 0; l--, ip++, jp--) {
            accum += *ip * *jp;
        }
        *--pDigit = accum % 10;
        accum /= 10;
    }
    result->exp = x->exp + y->exp;
    if (accum != 0) {
        *--pDigit = accum;
        result->exp++;
    }
    for (i = 0; i < 32 && pDigit < end; i++, pDigit++) {
        result->sig.text[i] = *pDigit;
    }
    result->sig.length = i;
    if (pDigit >= end) return;
    if (*pDigit < 5) return;
    if (*pDigit == 5) {
        u8 *q;
        for (q = pDigit + 1; q < end; q++) {
            if (*q != 0) goto round;
        }
        if (!(pDigit[-1] & 1)) return;
    }
round:
    func_0202d49c(result, result->sig.length);
}
