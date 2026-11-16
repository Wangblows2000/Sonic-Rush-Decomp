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
} UnkGXState_020036d0;

extern UnkGXState_020036d0 data_0207cfe8;

void GX_VRAMCNT_SetLCDC_(u32 lcdc);

#define VRAMCNT_A (*(vu8 *)0x04000240)
#define VRAMCNT_B (*(vu8 *)0x04000241)
#define VRAMCNT_C (*(vu8 *)0x04000242)
#define VRAMCNT_D (*(vu8 *)0x04000243)
#define VRAMCNT_E (*(vu8 *)0x04000244)
#define VRAMCNT_F (*(vu8 *)0x04000245)
#define VRAMCNT_G (*(vu8 *)0x04000246)
#define DISPCNT (*(vu32 *)0x04000000)

void func_020036d0(s32 v)
{
    data_0207cfe8.lcdc = (u16)(~v & (data_0207cfe8.lcdc | data_0207cfe8.objExtPltt));
    data_0207cfe8.objExtPltt = (u16)v;
    switch (v) {
    case 0x20:
        DISPCNT |= 0x80000000;
        VRAMCNT_F = 0x85;
        break;
    case 0x40:
        DISPCNT |= 0x80000000;
        VRAMCNT_G = 0x85;
        break;
    case 0:
        DISPCNT &= ~0x80000000;
        break;
    }
    GX_VRAMCNT_SetLCDC_(data_0207cfe8.lcdc);
}
