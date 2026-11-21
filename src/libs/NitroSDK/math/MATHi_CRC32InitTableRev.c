#include <nitro/types.h>


void MATHi_CRC32InitTableRev(u32 *table, u32 poly) {
    u32 r, i, j;
    for (i = 0; i < 0x100; i++) {
        r = i;
        for (j = 0; j < 8; j++) { if (r & 1) r = poly ^ (r >> 1); else r >>= 1; }
        table[i] = r;
    }
}
