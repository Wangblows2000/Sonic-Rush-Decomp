#include <nitro/types.h>

extern u16 data_0207cfe4;
extern u16 data_02077960;
void GX_DispOn(void) {
    data_02077960 = 1;
    if (data_0207cfe4) { *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x30000) | (data_0207cfe4 << 16); }
    else { *(vu32 *)0x4000000 |= 0x10000; }
}
