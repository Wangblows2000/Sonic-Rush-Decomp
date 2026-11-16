#include <nitro/types.h>

void GX_VRAMCNT_SetLCDC_(u32 lcdc)
{
    if (lcdc & 1) {
        *(vu8 *)0x04000240 = 0x80;
    }
    if (lcdc & 2) {
        *(vu8 *)0x04000241 = 0x80;
    }
    if (lcdc & 4) {
        *(vu8 *)0x04000242 = 0x80;
    }
    if (lcdc & 8) {
        *(vu8 *)0x04000243 = 0x80;
    }
    if (lcdc & 0x10) {
        *(vu8 *)0x04000244 = 0x80;
    }
    if (lcdc & 0x20) {
        *(vu8 *)0x04000245 = 0x80;
    }
    if (lcdc & 0x40) {
        *(vu8 *)0x04000246 = 0x80;
    }
    if (lcdc & 0x80) {
        *(vu8 *)0x04000248 = 0x80;
    }
    if (lcdc & 0x100) {
        *(vu8 *)0x04000249 = 0x80;
    }
}
