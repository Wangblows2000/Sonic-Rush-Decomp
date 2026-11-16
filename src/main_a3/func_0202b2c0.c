#include <nitro/types.h>

typedef struct {
    s8 sign;
    u8 unk1;
    s16 exp;
    struct {
        u8 length;
        u8 text[32];
        u8 unused;
    } sig;
} UnkDecimal_0202b2c0;

void func_0202b2c0(UnkDecimal_0202b2c0 *d, int digits)
{
    u8 *i;
    u8 *e;
    u8 *t;
    u8 c;
    int carry;

    if (digits < 0) {
    zero:
        d->exp = 0;
        d->sig.length = 1;
        d->sig.text[0] = '0';
        return;
    }
    if (digits >= d->sig.length)
        return;
    t = d->sig.text;
    i = t + digits + 1;
    c = *--i - '0';
    if (c == 5) {
        e = t + d->sig.length;
        while (--e > i && *e == '0') {
        }
        if (e == i)
            carry = i[-1] & 1;
        else
            carry = 1;
    } else {
        carry = c > 5;
    }
    while (digits != 0) {
        c = *--i - '0' + carry;
        carry = c > 9;
        if (carry || c == 0) {
            digits--;
        } else {
            *i = c + '0';
            break;
        }
    }
    if (carry) {
        d->exp++;
        d->sig.length = 1;
        d->sig.text[0] = '1';
        return;
    }
    if (digits == 0)
        goto zero;
    d->sig.length = digits;
}
