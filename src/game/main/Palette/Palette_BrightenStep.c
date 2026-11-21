#include <nitro/types.h>

void Palette_BrightenStep(u16 *p, u16 n) {
    while (n-- != 0) {
        *p += 1;
        *p += 0x20;
        *p += 0x400;
        p++;
    }
}
