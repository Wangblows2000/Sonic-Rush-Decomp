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
} UnkPrintFmt_0202ac68;

typedef struct {
    u8 style;
    u8 pad;
    s16 digits;
} UnkDecForm_0202ac68;

typedef struct {
    u8 sign;
    u8 unused;
    s16 exp;
    struct {
        u8 length;
        u8 text[36];
        u8 unused;
    } sig;
} UnkDecimal_0202ac68;

extern u16 data_02072b68[];
extern char data_02077f28[];
extern char data_02077f30[];
extern char data_02077f38[];
extern char data_02077f3c[];
extern char data_02077f40[];
extern char data_02077f48[];
extern char data_02077f50[];
extern char data_02077f54[];

void __num2dec(UnkDecForm_0202ac68 *form, double num, UnkDecimal_0202ac68 *dec);
void round_decimal(UnkDecimal_0202ac68 *dec, s32 digits);
char *strcpy(char *dst, const char *src);

static inline int IS_UPPER(int c)
{
    return (c < 0 || c >= 0x80) ? 0 : (data_02072b68[c] & 0x200);
}

char *float2str(double num, char *buf, UnkPrintFmt_0202ac68 fmt)
{
    UnkDecimal_0202ac68 dec;
    UnkDecForm_0202ac68 form;
    char *p;
    u8 *q;
    s32 n;
    s32 digits;
    s32 sign;
    s32 intDigits;
    s32 fracDigits;

    form.style = 0;
    form.digits = 0x20;
    __num2dec(&form, num, &dec);

    p = (char *)dec.sig.text + dec.sig.length;
    while (dec.sig.length > 1 && *(u8 *)--p == '0') {
        --dec.sig.length;
        ++dec.exp;
    }

    switch (dec.sig.text[0]) {
    case '0':
        dec.exp = 0;
        break;
    case 'I':
        if (num < 0) {
            p = buf - 5;
            if (IS_UPPER(fmt.conv))
                strcpy(p, data_02077f28);
            else
                strcpy(p, data_02077f30);
        } else {
            p = buf - 4;
            if (IS_UPPER(fmt.conv))
                strcpy(p, data_02077f38);
            else
                strcpy(p, data_02077f3c);
        }
        return p;
    case 'N':
        if (dec.sign) {
            p = buf - 5;
            if (IS_UPPER(fmt.conv))
                strcpy(p, data_02077f40);
            else
                strcpy(p, data_02077f48);
        } else {
            p = buf - 4;
            if (IS_UPPER(fmt.conv))
                strcpy(p, data_02077f50);
            else
                strcpy(p, data_02077f54);
        }
        return p;
    }

    dec.exp += dec.sig.length - 1;
    p = buf;
    *--p = 0;

    switch (fmt.conv) {
    case 'g':
    case 'G':
        if (dec.sig.length > fmt.prec)
            round_decimal(&dec, fmt.prec);
        if (dec.exp < -4 || dec.exp >= fmt.prec) {
            if (fmt.alt)
                --fmt.prec;
            else
                fmt.prec = dec.sig.length - 1;
            if (fmt.conv == 'g')
                fmt.conv = 'e';
            else
                fmt.conv = 'E';
            goto e_format;
        }
        if (fmt.alt)
            fmt.prec -= dec.exp + 1;
        else if ((fmt.prec = dec.sig.length - (dec.exp + 1)) < 0)
            fmt.prec = 0;
        goto f_format;

    case 'e':
    case 'E':
    e_format:
        if (dec.sig.length > fmt.prec + 1)
            round_decimal(&dec, fmt.prec + 1);
        n = dec.exp;
        sign = '+';
        if (n < 0) {
            n = -n;
            sign = '-';
        }
        for (digits = 0; n || digits < 2; ++digits) {
            *--p = n % 10 + '0';
            n /= 10;
        }
        *--p = sign;
        *--p = fmt.conv;
        if (buf - p + fmt.prec > 509)
            return NULL;
        if (dec.sig.length < fmt.prec + 1) {
            for (n = fmt.prec + 1 - dec.sig.length + 1; --n;)
                *--p = '0';
        }
        for (n = dec.sig.length, q = dec.sig.text + dec.sig.length; --n;)
            *--p = *--q;
        if (fmt.prec || fmt.alt)
            *--p = '.';
        *--p = dec.sig.text[0];
        if (dec.sign)
            *--p = '-';
        else if (fmt.sign == 1)
            *--p = '+';
        else if (fmt.sign == 2)
            *--p = ' ';
        break;

    case 'f':
    case 'F':
    f_format:
        if ((fracDigits = -dec.exp + dec.sig.length - 1) < 0)
            fracDigits = 0;
        if (fracDigits > fmt.prec) {
            round_decimal(&dec, dec.sig.length - (fracDigits - fmt.prec));
            if ((fracDigits = -dec.exp + dec.sig.length - 1) < 0)
                fracDigits = 0;
        }
        if ((intDigits = dec.exp + 1) < 0)
            intDigits = 0;
        if (intDigits + fracDigits > 509)
            return NULL;
        q = dec.sig.text + dec.sig.length;
        for (digits = 0; digits < fmt.prec - fracDigits; ++digits)
            *--p = '0';
        for (digits = 0; digits < fracDigits && digits < dec.sig.length; ++digits)
            *--p = *--q;
        for (; digits < fracDigits; ++digits)
            *--p = '0';
        if (fmt.prec || fmt.alt)
            *--p = '.';
        if (intDigits) {
            for (digits = 0; digits < intDigits - dec.sig.length; ++digits)
                *--p = '0';
            for (; digits < intDigits; ++digits)
                *--p = *--q;
        } else {
            *--p = '0';
        }
        if (dec.sign)
            *--p = '-';
        else if (fmt.sign == 1)
            *--p = '+';
        else if (fmt.sign == 2)
            *--p = ' ';
        break;
    }
    return p;
}
