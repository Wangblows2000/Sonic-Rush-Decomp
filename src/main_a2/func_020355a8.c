#include <nitro/types.h>

extern u16 data_020730ac[3][32];

void func_020355a8(u16 *pal, u16 n) {
    while (n--) {
        u16 c = *pal;
        *pal = (data_020730ac[0][c & 0x1f] + data_020730ac[1][(c >> 5) & 0x1f] + data_020730ac[2][(c >> 10) & 0x1f]) >> 8;
        *pal = *pal | ((*pal << 5) | (*pal << 10));
        *pal++ ^= 0x7fff;
    }
}
