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
} UnkDecimal_0202cd1c;

extern const char data_02077f60[];
extern const char data_02077f90[];
extern const char data_02077fb8[];
extern const char data_02077fd0[];
extern const char data_02077fe0[];
extern const char data_02077fe8[];
extern const char data_02077ff0[];
extern const char data_02077ff8[];
extern const char data_02078000[];
extern const char data_02078004[];
extern const char data_02078008[];
extern const char data_0207800c[];
extern const char data_02078010[];
extern const char data_02078014[];
extern const char data_02078018[];
extern const char data_0207801c[];
extern const char data_02078020[];
extern const char data_02078024[];
extern const char data_02078028[];
extern const char data_0207802c[];
extern const char data_02078030[];

void func_0202d0fc(UnkDecimal_0202cd1c *d, const char *s, s16 exp);
void __timesdec(UnkDecimal_0202cd1c *result, const UnkDecimal_0202cd1c *x, const UnkDecimal_0202cd1c *y);

void __two_exp(UnkDecimal_0202cd1c *result, s32 n) {
    switch (n) {
    case -64:
        func_0202d0fc(result, data_02077f60, -20);
        return;
    case -53:
        func_0202d0fc(result, data_02077f90, -16);
        return;
    case -32:
        func_0202d0fc(result, data_02077fb8, -10);
        return;
    case -16:
        func_0202d0fc(result, data_02077fd0, -5);
        return;
    case -8:
        func_0202d0fc(result, data_02077fe0, -3);
        return;
    case -7:
        func_0202d0fc(result, data_02077fe8, -3);
        return;
    case -6:
        func_0202d0fc(result, data_02077ff0, -2);
        return;
    case -5:
        func_0202d0fc(result, data_02077ff8, -2);
        return;
    case -4:
        func_0202d0fc(result, data_02078000, -2);
        return;
    case -3:
        func_0202d0fc(result, data_02078004, -1);
        return;
    case -2:
        func_0202d0fc(result, data_02078008, -1);
        return;
    case -1:
        func_0202d0fc(result, data_0207800c, -1);
        return;
    case 0:
        func_0202d0fc(result, data_02078010, 0);
        return;
    case 1:
        func_0202d0fc(result, data_02078014, 0);
        return;
    case 2:
        func_0202d0fc(result, data_02078018, 0);
        return;
    case 3:
        func_0202d0fc(result, data_0207801c, 0);
        return;
    case 4:
        func_0202d0fc(result, data_02078020, 1);
        return;
    case 5:
        func_0202d0fc(result, data_02078024, 1);
        return;
    case 6:
        func_0202d0fc(result, data_02078028, 1);
        return;
    case 7:
        func_0202d0fc(result, data_0207802c, 2);
        return;
    case 8:
        func_0202d0fc(result, data_02078030, 2);
        return;
    }
    {
        UnkDecimal_0202cd1c x;
        UnkDecimal_0202cd1c y;
        __two_exp(&x, (n + (s32)((u32)(n & 0x80000000) >> 31)) >> 1);
        __timesdec(result, &x, &x);
        if (n & 1) {
            y = *result;
            if (n > 0) {
                func_0202d0fc(&x, data_02078014, 0);
            } else {
                func_0202d0fc(&x, data_0207800c, -1);
            }
            __timesdec(result, &y, &x);
        }
    }
}
