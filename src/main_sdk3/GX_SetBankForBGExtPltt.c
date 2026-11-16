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
} UnkGXState_02003788;

extern UnkGXState_02003788 data_0207cfe8;

void GX_VRAMCNT_SetLCDC_(u32 lcdc);

#define VRAMCNT_E (*(vu8 *)0x04000244)
#define VRAMCNT_F (*(vu8 *)0x04000245)
#define VRAMCNT_G (*(vu8 *)0x04000246)
#define DISPCNT (*(vu32 *)0x04000000)

void GX_SetBankForBGExtPltt(s32 bgExtPltt)
{
    data_0207cfe8.lcdc = (u16)(~bgExtPltt & (data_0207cfe8.lcdc | data_0207cfe8.bgExtPltt));
    data_0207cfe8.bgExtPltt = (u16)bgExtPltt;
    switch (bgExtPltt) {
    case 0x10:
        DISPCNT |= 0x40000000;
        VRAMCNT_E = 0x84;
        break;
    case 0x40:
        DISPCNT |= 0x40000000;
        VRAMCNT_G = 0x8c;
        break;
    case 0x60:
        VRAMCNT_G = 0x8c;
    case 0x20:
        VRAMCNT_F = 0x84;
        DISPCNT |= 0x40000000;
        break;
    case 0:
        DISPCNT &= ~0x40000000;
        break;
    }
    GX_VRAMCNT_SetLCDC_(data_0207cfe8.lcdc);
}
