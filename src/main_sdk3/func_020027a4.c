#include <nitro/types.h>

extern const s16 data_02072470[];

fx32 func_0200225c(fx32 numer, fx32 denom);

u16 func_020027a4(fx32 y, fx32 x)
{
    fx32 a, b;
    s32 c;
    BOOL positive;

    if (y > 0) {
        if (x > 0) {
            if (x > y) {
                a = y;
                b = x;
                c = 0;
                positive = TRUE;
            } else if (x < y) {
                a = x;
                b = y;
                c = 16384;
                positive = FALSE;
            } else {
                return (u16)8192;
            }
        } else if (x < 0) {
            x = -x;
            if (x < y) {
                a = x;
                b = y;
                c = 16384;
                positive = TRUE;
            } else if (x > y) {
                a = y;
                b = x;
                c = 32768;
                positive = FALSE;
            } else {
                return (u16)24576;
            }
        } else {
            return (u16)16384;
        }
    } else if (y < 0) {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x > y) {
                a = y;
                b = x;
                c = -32768;
                positive = TRUE;
            } else if (x < y) {
                a = x;
                b = y;
                c = -16384;
                positive = FALSE;
            } else {
                return (u16)40960;
            }
        } else if (x > 0) {
            if (x < y) {
                a = x;
                b = y;
                c = -16384;
                positive = TRUE;
            } else if (x > y) {
                a = y;
                b = x;
                c = 0;
                positive = FALSE;
            } else {
                return (u16)57344;
            }
        } else {
            return (u16)49152;
        }
    } else {
        if (x >= 0) {
            return 0;
        } else {
            return (u16)32768;
        }
    }
    if (b == 0) {
        return 0;
    }
    if (positive) {
        return (u16)(c + data_02072470[func_0200225c(a, b) >> 5]);
    } else {
        return (u16)(c - data_02072470[func_0200225c(a, b) >> 5]);
    }
}
