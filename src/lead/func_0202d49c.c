#include <nitro/types.h>

typedef struct {
    u8 sign;
    u8 unused;
    s16 exp;
    struct {
        u8 length;
        u8 text[32];
        u8 unused;
    } sig;
} UnkDecimal_0202d49c;

void func_0202d49c(UnkDecimal_0202d49c *d, s32 digits)
{
    u8 *b = d->sig.text;
    u8 *i = b + digits - 1;

loop:
    if (*i < 9) {
        (*i)++;
        return;
    }
    if (i == b) {
        *i = 1;
        d->exp++;
        return;
    }
    *i-- = 0;
    goto loop;
}
