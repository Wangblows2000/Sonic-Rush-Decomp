#include <nitro/types.h>

typedef struct {
    u16 lcdc;
    u16 bg;
    u16 obj;
    u16 arm7;
    u16 tex;
} UnkGXState_020034cc;

extern UnkGXState_020034cc data_0207cfe8;

void GX_VRAMCNT_SetLCDC_(u32 lcdc);

#define VRAMCNT_A (*(vu8 *)0x04000240)
#define VRAMCNT_B (*(vu8 *)0x04000241)
#define VRAMCNT_C (*(vu8 *)0x04000242)
#define VRAMCNT_D (*(vu8 *)0x04000243)
#define DISP3DCNT (*(vu16 *)0x04000060)

void GX_SetBankForTex(s32 tex)
{
    data_0207cfe8.lcdc = (u16)(~tex & (data_0207cfe8.lcdc | data_0207cfe8.tex));
    data_0207cfe8.tex = (u16)tex;
    if (tex == 0) {
        DISP3DCNT &= (u16)~0x3001;
    } else {
        DISP3DCNT = (u16)((DISP3DCNT & ~0x3000) | 1);
        switch (tex) {
        case 5:
            VRAMCNT_A = 0x83;
            VRAMCNT_C = 0x8b;
            break;
        case 9:
            VRAMCNT_A = 0x83;
            VRAMCNT_D = 0x8b;
            break;
        case 10:
            VRAMCNT_B = 0x83;
            VRAMCNT_D = 0x8b;
            break;
        case 11:
            VRAMCNT_A = 0x83;
            VRAMCNT_B = 0x8b;
            VRAMCNT_D = 0x93;
            break;
        case 13:
            VRAMCNT_A = 0x83;
            VRAMCNT_C = 0x8b;
            VRAMCNT_D = 0x93;
            break;
        case 8:
            VRAMCNT_D = 0x83;
            break;
        case 12:
            VRAMCNT_D = 0x8b;
        case 4:
            VRAMCNT_C = 0x83;
            break;
        case 14:
            VRAMCNT_D = 0x93;
        case 6:
            VRAMCNT_C = 0x8b;
        case 2:
            VRAMCNT_B = 0x83;
            break;
        case 15:
            VRAMCNT_D = 0x9b;
        case 7:
            VRAMCNT_C = 0x93;
        case 3:
            VRAMCNT_B = 0x8b;
        case 1:
            VRAMCNT_A = 0x83;
            break;
        case 0:
        default:
            break;
        }
    }
    GX_VRAMCNT_SetLCDC_(data_0207cfe8.lcdc);
}
