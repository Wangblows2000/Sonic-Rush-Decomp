#include <nitro/types.h>

void Palette_Invert(u16 *p, u16 n) {
    while (n-- != 0) {
        *p++ ^= 0x7FFF;
    }
}
