#include <nitro/types.h>

typedef struct {
    u16 lcdc;
    u16 bg;
    u16 obj;
    u16 arm7;
    u16 tex;
    u16 texPltt;
    u16 clrImg;
    u16 bgExtPltt;
    u16 objExtPltt;
} UnkGXState_020033d8;

extern UnkGXState_020033d8 data_0207cfe8;

void GX_VRAMCNT_SetLCDC_(u32 lcdc);

#define VRAMCNT_A (*(vu8 *)0x04000240)
#define VRAMCNT_B (*(vu8 *)0x04000241)
#define VRAMCNT_C (*(vu8 *)0x04000242)
#define VRAMCNT_D (*(vu8 *)0x04000243)
#define VRAMCNT_E (*(vu8 *)0x04000244)
#define VRAMCNT_F (*(vu8 *)0x04000245)
#define VRAMCNT_G (*(vu8 *)0x04000246)
#define DISPCNT (*(vu32 *)0x04000000)

void func_020033d8(s32 v)
{
    data_0207cfe8.lcdc = (u16)(~v & (data_0207cfe8.lcdc | data_0207cfe8.texPltt));
    data_0207cfe8.texPltt = (u16)v;
    switch (v) {
    case 0x60:
        VRAMCNT_G = 0x8b;
    case 0x20:
        VRAMCNT_F = 0x83;
        break;
    case 0x70:
        VRAMCNT_G = 0x9b;
    case 0x30:
        VRAMCNT_F = 0x93;
    case 0x10:
        VRAMCNT_E = 0x83;
        break;
    case 0x40:
        VRAMCNT_G = 0x83;
        break;
    case 0:
        break;
    }
    GX_VRAMCNT_SetLCDC_(data_0207cfe8.lcdc);
}
