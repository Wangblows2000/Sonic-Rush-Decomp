#include <nitro/types.h>

/* NOTE: compiler emits calls to _ull_mod/_ll_udiv = _ull_mod/_ll_udiv (symbol rename needed for link). */

typedef struct {
    s8 sign;
    u8 unused;
    s16 exp;
    struct {
        u8 length;
        u8 text[32];
        u8 unused;
    } sig;
} UnkDecimal_0202d368;

void func_0202d368(UnkDecimal_0202d368 *d, u64 val) {
    u8 *p;
    u8 *q;
    d->sign = 0;
    d->sig.length = 0;
    for (; val != 0; val /= 10) {
        d->sig.text[d->sig.length++] = val % 10;
    }
    p = d->sig.text;
    q = p + d->sig.length - 1;
    for (; p < q; p++, q--) {
        u8 t = *p;
        *p = *q;
        *q = t;
    }
    d->exp = d->sig.length - 1;
}
