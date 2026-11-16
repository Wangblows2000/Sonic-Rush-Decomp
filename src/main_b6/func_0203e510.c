#include <nitro/types.h>

extern u32 data_022c1988;

s16 func_0203e510(u16 mask) {
    s16 i;
    s16 last = 0;
    u16 cnt = 0;
    u16 n;

    for (i = 0; i < 16; i++) {
        if (mask & (1 << i)) {
            last = i + 1;
            cnt++;
        }
    }
    if (cnt <= 1)
        return last;
    data_022c1988 = data_022c1988 * 0x10dcd + 0x3039;
    n = (cnt * (data_022c1988 & 0xff)) >> 8;
    for (i = 0; i < 16; i++) {
        if (mask & 1) {
            if (n == 0)
                return i + 1;
            n--;
        }
        mask >>= 1;
    }
    return 0;
}
