#include <nitro/types.h>

typedef struct {
    u16 lcdc;
    u16 bg;
} UnkGXState_02003a08;

extern UnkGXState_02003a08 data_0207cfe8;

void GX_VRAMCNT_SetLCDC_(u32 lcdc);

#define VRAMCNT_A (*(vu8 *)0x04000240)
#define VRAMCNT_B (*(vu8 *)0x04000241)
#define VRAMCNT_C (*(vu8 *)0x04000242)
#define VRAMCNT_D (*(vu8 *)0x04000243)
#define VRAMCNT_E (*(vu8 *)0x04000244)
#define VRAMCNT_F (*(vu8 *)0x04000245)
#define VRAMCNT_G (*(vu8 *)0x04000246)

void func_02003a08(s32 bg)
{
    data_0207cfe8.lcdc = (u16)(~bg & (data_0207cfe8.lcdc | data_0207cfe8.bg));
    data_0207cfe8.bg = (u16)bg;
    switch (bg) {
    case 8:
        VRAMCNT_D = 0x81;
        break;
    case 12:
        VRAMCNT_D = 0x89;
    case 4:
        VRAMCNT_C = 0x81;
        break;
    case 14:
        VRAMCNT_D = 0x91;
    case 6:
        VRAMCNT_C = 0x89;
    case 2:
        VRAMCNT_B = 0x81;
        break;
    case 15:
        VRAMCNT_D = 0x99;
    case 7:
        VRAMCNT_C = 0x91;
    case 3:
        VRAMCNT_B = 0x89;
    case 1:
        VRAMCNT_A = 0x81;
        break;
    case 11:
        VRAMCNT_A = 0x81;
        VRAMCNT_B = 0x89;
        VRAMCNT_D = 0x91;
        break;
    case 13:
        VRAMCNT_D = 0x91;
    case 5:
        VRAMCNT_A = 0x81;
        VRAMCNT_C = 0x89;
        break;
    case 9:
        VRAMCNT_A = 0x81;
        VRAMCNT_D = 0x89;
        break;
    case 10:
        VRAMCNT_B = 0x81;
        VRAMCNT_D = 0x89;
        break;
    case 0x70:
        VRAMCNT_G = 0x99;
    case 0x30:
        VRAMCNT_F = 0x91;
    case 0x10:
        VRAMCNT_E = 0x81;
        break;
    case 0x50:
        VRAMCNT_G = 0x91;
        VRAMCNT_E = 0x81;
        break;
    case 0x60:
        VRAMCNT_G = 0x89;
    case 0x20:
        VRAMCNT_F = 0x81;
        break;
    case 0x40:
        VRAMCNT_G = 0x81;
        break;
    case 0:
    default:
        break;
    }
    GX_VRAMCNT_SetLCDC_(data_0207cfe8.lcdc);
}
