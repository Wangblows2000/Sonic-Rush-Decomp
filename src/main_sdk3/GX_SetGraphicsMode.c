#include <nitro/types.h>

extern u16 data_02077960;
extern u16 data_0207cfe4;

void GX_SetGraphicsMode(u32 dispMode, u32 bgMode, u32 bg0_2d3d)
{
    u32 reg = *(vu32 *)0x04000000;
    data_0207cfe4 = (u16)dispMode;
    if (!data_02077960) {
        dispMode = 0;
    }
    *(vu32 *)0x04000000 = (reg & ~0x000f000f) | (dispMode << 16) | bgMode | (bg0_2d3d << 3);
    if (!data_0207cfe4) {
        data_02077960 = 0;
    }
}
