#include <nitro/types.h>

typedef struct {
    u8 justify;
    u8 sign;
    u8 precSet;
    u8 alt;
    u8 argOpt;
    u8 conv;
    s32 width;
    s32 prec;
} UnkPrintFmt_0202ba98;

char *func_0202ba98(s32 num, char *buf, UnkPrintFmt_0202ba98 fmt)
{
    u32 unum;
    u32 base;
    char *p;
    s32 n;
    s32 digits;
    s32 minus;

    unum = num;
    minus = 0;
    p = buf;
    *--p = 0;
    digits = 0;

    if (num == 0 && fmt.prec == 0 && !(fmt.alt && fmt.conv == 'o'))
        return p;

    switch (fmt.conv) {
    case 'd':
    case 'i':
        base = 10;
        if (num < 0) {
            unum = -unum;
            minus = 1;
        }
        break;
    case 'o':
        base = 8;
        fmt.sign = 0;
        break;
    case 'u':
        base = 10;
        fmt.sign = 0;
        break;
    case 'x':
    case 'X':
        base = 16;
        fmt.sign = 0;
        break;
    }

    do {
        n = unum % base;
        unum /= base;
        if (n < 10) {
            n += '0';
        } else {
            n -= 10;
            if (fmt.conv == 'x')
                n += 'a';
            else
                n += 'A';
        }
        *--p = n;
        ++digits;
    } while (unum != 0);

    if (base == 8 && fmt.alt && *(u8 *)p != '0') {
        *--p = '0';
        ++digits;
    }

    if (fmt.justify == 2) {
        fmt.prec = fmt.width;
        if (minus || fmt.sign != 0)
            --fmt.prec;
        if (base == 16 && fmt.alt)
            fmt.prec -= 2;
    }

    if (buf - p + fmt.prec > 509)
        return NULL;

    while (digits < fmt.prec) {
        *--p = '0';
        ++digits;
    }

    if (base == 16 && fmt.alt) {
        *--p = fmt.conv;
        *--p = '0';
    }

    if (minus)
        *--p = '-';
    else if (fmt.sign == 1)
        *--p = '+';
    else if (fmt.sign == 2)
        *--p = ' ';

    return p;
}
