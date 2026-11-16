#include <nitro/types.h>

u32 func_02002094(u32 a, u32 b);

void func_ov000_02336f28(u32 *out, u32 v, u32 n) {
    s16 sh = 0;
    u32 q;
    s16 i;
    u32 o;
    s32 j;

    if (v >= n) {
        if (n == 0)
            v = 0;
        else
            v = n - 1;
    }
    *out = 0;
    if (n >= 100) {
        n = func_02002094(n, 10);
        sh = 0;
        for (;;) {
            if (v >= n) {
                q = func_02002094(v, n);
                *out |= q << sh;
                v -= q * n;
            }
            sh += 4;
            if (n <= 10)
                break;
            n = func_02002094(n, 10);
        }
    }
    *out |= v << sh;
    for (i = sh; i >= sh >> 1; i -= 4) {
        o = *out;
        j = sh - i;
        *out = o & ~((0xf << i) | (0xf << j));
        *out |= (((o & (0xf << j)) >> j) << i) | (((o & (0xf << i)) >> i) << j);
    }
}
