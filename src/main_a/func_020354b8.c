#include <nitro/types.h>

void func_020354b8(u16 *p, u16 n) {
    while (n-- != 0) {
        *p += 1;
        *p += 0x20;
        *p += 0x400;
        p++;
    }
}
