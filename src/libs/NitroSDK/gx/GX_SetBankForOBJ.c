#include <nitro/types.h>

typedef struct {
    u16 lcdc;
    u16 bg;
    u16 obj;
} UnkGXState_0200389c;

extern UnkGXState_0200389c data_0207cfe8;

void GX_VRAMCNT_SetLCDC_(u32 lcdc);

#define VRAMCNT_A (*(vu8 *)0x04000240)
#define VRAMCNT_B (*(vu8 *)0x04000241)
#define VRAMCNT_E (*(vu8 *)0x04000244)
#define VRAMCNT_F (*(vu8 *)0x04000245)
#define VRAMCNT_G (*(vu8 *)0x04000246)

void GX_SetBankForOBJ(s32 obj)
{
    data_0207cfe8.lcdc = (u16)(~obj & (data_0207cfe8.lcdc | data_0207cfe8.obj));
    data_0207cfe8.obj = (u16)obj;
    switch (obj) {
    case 3:
        VRAMCNT_B = 0x8a;
    case 1:
        VRAMCNT_A = 0x82;
        break;
    case 2:
        VRAMCNT_B = 0x82;
        break;
    case 0x70:
        VRAMCNT_G = 0x9a;
    case 0x30:
        VRAMCNT_F = 0x92;
    case 0x10:
        VRAMCNT_E = 0x82;
        break;
    case 0x50:
        VRAMCNT_G = 0x92;
        VRAMCNT_E = 0x82;
        break;
    case 0x60:
        VRAMCNT_G = 0x8a;
    case 0x20:
        VRAMCNT_F = 0x82;
        break;
    case 0x40:
        VRAMCNT_G = 0x82;
        break;
    case 0:
    default:
        break;
    }
    GX_VRAMCNT_SetLCDC_(data_0207cfe8.lcdc);
}
