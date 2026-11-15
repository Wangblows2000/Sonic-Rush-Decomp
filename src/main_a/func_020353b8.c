#include <nitro/types.h>

void func_020353b8(u16 *p, u16 n) {
    while (n-- != 0) {
        if (*p & 0x1F) *p -= 1;
        if (*p & 0x3E0) *p -= 0x20;
        if (*p & 0x7C00) *p -= 0x400;
        p++;
    }
}
