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
} UnkDecimal_0202d0fc;

void __dorounddecup(UnkDecimal_0202d0fc *d, s32 n);

void func_0202d0fc(UnkDecimal_0202d0fc *d, const u8 *s, s16 exp) {
    s32 i;
    d->exp = exp;
    d->sign = 0;
    for (i = 0; i < 32 && *s != 0;) {
        d->sig.text[i++] = *s++ - '0';
    }
    d->sig.length = i;
    if (*s == 0) return;
    if (*s < 5) return;
    if (*s <= 5) {
        const u8 *p = s + 1;
        while (*p != 0) {
            if (*p != '0') goto round;
            p++;
        }
        if (!(d->sig.text[(u32)(i - 1)] & 1)) return;
    }
round:
    __dorounddecup(d, d->sig.length);
}
