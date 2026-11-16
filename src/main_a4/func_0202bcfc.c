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
} UnkPrintFmt_0202bcfc;

extern u16 data_02072b68[];

#define IS_DIGIT(c) (((c) < 0 || (c) >= 0x80) ? 0 : (data_02072b68[c] & 8))
#define VA_INT(ap) (*(int *)((*(ap) += 4) - 4))

const u8 *func_0202bcfc(const u8 *str, char **arg, UnkPrintFmt_0202bcfc *out)
{
    UnkPrintFmt_0202bcfc f;
    const u8 *s;
    int c;
    int found;

    s = str;
    f.justify = 1;
    f.sign = 0;
    f.precSet = 0;
    f.alt = 0;
    f.argOpt = 0;
    f.width = 0;
    f.prec = 0;

    if ((c = *++s) == '%') {
        f.conv = c;
        *out = f;
        return s + 1;
    }

    for (;;) {
        found = 1;
        switch (c) {
        case '-':
            f.justify = 0;
            break;
        case '+':
            f.sign = 1;
            break;
        case ' ':
            if (f.sign != 1)
                f.sign = 2;
            break;
        case '#':
            f.alt = 1;
            break;
        case '0':
            if (f.justify != 0)
                f.justify = 2;
            break;
        default:
            found = 0;
            break;
        }
        if (found)
            c = *++s;
        else
            break;
    }

    if (c == '*') {
        if ((f.width = VA_INT(arg)) < 0) {
            f.justify = 0;
            f.width = -f.width;
        }
        c = *++s;
    } else {
        while (IS_DIGIT(c)) {
            f.width = f.width * 10 + (c - '0');
            c = *++s;
        }
    }

    if (f.width > 509) {
        f.conv = 0xff;
        *out = f;
        return s + 1;
    }

    if (c == '.') {
        f.precSet = 1;
        if ((c = *++s) == '*') {
            if ((f.prec = VA_INT(arg)) < 0)
                f.precSet = 0;
            c = *++s;
        } else {
            while (IS_DIGIT(c)) {
                f.prec = f.prec * 10 + (c - '0');
                c = *++s;
            }
        }
    }

    found = 1;
    switch (c) {
    case 'h':
        f.argOpt = 2;
        if (s[1] == 'h') {
            f.argOpt = 1;
            c = *++s;
        }
        break;
    case 'l':
        f.argOpt = 3;
        if (s[1] == 'l') {
            f.argOpt = 7;
            c = *++s;
        }
        break;
    case 'L':
        f.argOpt = 8;
        break;
    case 'j':
        f.argOpt = 4;
        break;
    case 'z':
        f.argOpt = 5;
        break;
    case 't':
        f.argOpt = 6;
        break;
    default:
        found = 0;
        break;
    }
    if (found)
        c = *++s;

    f.conv = c;
    switch (c) {
    case 'd':
    case 'i':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
        if (f.argOpt == 8) {
            f.conv = 0xff;
            break;
        }
        if (!f.precSet)
            f.prec = 1;
        else if (f.justify == 2)
            f.justify = 1;
        break;
    case 'f':
    case 'F':
        if (f.argOpt == 2 || f.argOpt == 4 || f.argOpt == 5 || f.argOpt == 6 || f.argOpt == 7) {
            f.conv = 0xff;
            break;
        }
        if (!f.precSet)
            f.prec = 6;
        break;
    case 'a':
    case 'A':
        if (!f.precSet)
            f.prec = 13;
        if (f.argOpt == 2 || f.argOpt == 4 || f.argOpt == 5 || f.argOpt == 6 || f.argOpt == 7 || f.argOpt == 1)
            f.conv = 0xff;
        break;
    case 'g':
    case 'G':
        if (f.prec == 0)
            f.prec = 1;
    case 'e':
    case 'E':
        if (f.argOpt == 2 || f.argOpt == 4 || f.argOpt == 5 || f.argOpt == 6 || f.argOpt == 7 || f.argOpt == 1) {
            f.conv = 0xff;
            break;
        }
        if (!f.precSet)
            f.prec = 6;
        break;
    case 'p':
        f.conv = 'x';
        f.alt = 1;
        f.argOpt = 3;
        f.prec = 8;
        break;
    case 'c':
        if (f.argOpt == 3) {
            f.argOpt = 9;
            break;
        }
        if (f.precSet || f.argOpt != 0)
            f.conv = 0xff;
        break;
    case 's':
        if (f.argOpt == 3) {
            f.argOpt = 9;
            break;
        }
        if (f.argOpt != 0)
            f.conv = 0xff;
        break;
    case 'n':
        if (f.argOpt == 8)
            f.conv = 0xff;
        break;
    default:
        f.conv = 0xff;
        break;
    }

    *out = f;
    return s + 1;
}
