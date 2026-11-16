#include <nitro/types.h>

extern u16 gDisplayMain[];

void Ending_Unk_ov001_022c7d2c(void) {
    gDisplayMain[0] = 0;
    gDisplayMain[1] = 0xff70;
    gDisplayMain[4] = 0xffba;
    gDisplayMain[5] = 0;
}
