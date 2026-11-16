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
} UnkPrintFmt_0202b3f0;

typedef struct {
    u8 style;
    u8 pad;
    s16 digits;
} UnkDecForm_0202b3f0;

typedef struct {
    u8 sign;
    u8 unused;
    s16 exp;
    struct {
        u8 length;
        u8 text[32];
        u8 unused;
    } sig;
} UnkDecimal_0202b3f0;

extern char data_02077f28[];
extern char data_02077f30[];
extern char data_02077f38[];
extern char data_02077f3c[];
extern char data_02077f40[];
extern char data_02077f48[];
extern char data_02077f50[];
extern char data_02077f54[];
extern char data_02077f58[];

void func_0202c9fc(UnkDecForm_0202b3f0 *form, double num, UnkDecimal_0202b3f0 *dec);
double func_0202c634(double x);
char *func_0202c518(char *dst, const char *src);
char *func_0202ba98(s32 num, char *buf, UnkPrintFmt_0202b3f0 fmt);

char *func_0202b3f0(double num, char *buf, UnkPrintFmt_0202b3f0 fmt)
{
    u8 *h;
    u8 wb;
    u8 t;
    UnkPrintFmt_0202b3f0 expFmt;
    s32 i;
    UnkDecForm_0202b3f0 form;
    UnkDecimal_0202b3f0 dec;
    u8 *q;
    s32 hexPrec;
    s32 exp;
    char *p;
    s32 bit;

    if (fmt.prec > 509)
        return NULL;

    form.style = 0;
    form.digits = 0x20;
    func_0202c9fc(&form, num, &dec);

    if (0.0 == func_0202c634(num)) {
        p = buf - 6;
        func_0202c518(p, data_02077f58);
        return p;
    }

    if (dec.sig.text[0] == 'I') {
        if (dec.sign) {
            p = buf - 5;
            if (fmt.conv == 'A')
                func_0202c518(p, data_02077f28);
            else
                func_0202c518(p, data_02077f30);
        } else {
            p = buf - 4;
            if (fmt.conv == 'A')
                func_0202c518(p, data_02077f38);
            else
                func_0202c518(p, data_02077f3c);
        }
        return p;
    }
    if (dec.sig.text[0] == 'N') {
        if (dec.sign) {
            p = buf - 5;
            if (fmt.conv == 'A')
                func_0202c518(p, data_02077f40);
            else
                func_0202c518(p, data_02077f48);
        } else {
            p = buf - 4;
            if (fmt.conv == 'A')
                func_0202c518(p, data_02077f50);
            else
                func_0202c518(p, data_02077f54);
        }
        return p;
    }

    expFmt.justify = 1;
    expFmt.sign = 1;
    expFmt.precSet = 0;
    expFmt.alt = 0;
    expFmt.argOpt = 0;
    expFmt.width = 0;
    expFmt.prec = 1;
    expFmt.conv = 'd';

    q = (u8 *)&num;
    for (i = 0; i < 4; i++) {
        t = q[i];
        q[i] = q[7 - i];
        q[7 - i] = t;
    }

    exp = (u32)((u8 *)&num)[0] << 25;
    exp |= (u32)((u8 *)&num)[1] << 17;
    exp = ((u32)exp >> 21) & 0x7ff;
    p = func_0202ba98(exp - 0x3ff, buf, expFmt);
    if (fmt.conv == 'a')
        *--p = 'p';
    else
        *--p = 'P';

    h = (u8 *)&num;
    for (hexPrec = fmt.prec, bit = fmt.prec * 4 + 11; hexPrec >= 1; --hexPrec, bit -= 4) {
        if (bit < 64) {
            wb = h[bit >> 3] >> (7 - (bit & 7));
            if ((bit & ~7) != ((bit - 4) & ~7))
                wb |= (h[(int)((bit >> 3) - 1)] << 8) >> (7 - (bit & 7));
            wb &= 0x0f;
            if (wb < 10)
                wb += '0';
            else if (fmt.conv == 'a')
                wb += 'a' - 10;
            else
                wb += 'A' - 10;
        } else {
            wb = '0';
        }
        *--p = wb;
    }

    if (fmt.prec || fmt.alt)
        *--p = '.';
    *--p = '1';
    if (fmt.conv == 'a')
        *--p = 'x';
    else
        *--p = 'X';
    *--p = '0';

    if (dec.sign)
        *--p = '-';
    else if (fmt.sign == 1)
        *--p = '+';
    else if (fmt.sign == 2)
        *--p = ' ';
    return p;
}
