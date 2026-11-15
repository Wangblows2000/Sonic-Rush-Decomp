#include <nitro/types.h>

void func_02035660(u16 *p, u16 n) {
    while (n-- != 0) {
        *p++ ^= 0x7FFF;
    }
}
