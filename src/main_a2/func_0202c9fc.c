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
} UnkDecimal_0202c9fc;

typedef struct { u8 unk0[2]; s16 prec; } UnkForm_0202c9fc;

void func_0202cac4(UnkDecimal_0202c9fc *d, double x);
void func_0202d438(UnkDecimal_0202c9fc *d, s32 n);

void func_0202c9fc(const UnkForm_0202c9fc *form, double x, UnkDecimal_0202c9fc *d) {
    s32 prec = form->prec;
    s32 i;
    func_0202cac4(d, x);
    if (d->sig.text[0] > 9) return;
    if (prec > 32) prec = 32;
    func_0202d438(d, prec);
    while (d->sig.length < prec) {
        d->sig.text[d->sig.length++] = 0;
    }
    d->exp -= d->sig.length - 1;
    for (i = 0; i < d->sig.length; i++) {
        d->sig.text[i] += '0';
    }
}
