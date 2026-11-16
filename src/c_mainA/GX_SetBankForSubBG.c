#include <nitro/types.h>
typedef struct { u16 lcdc; u16 bg; u16 obj; u16 arm7; u16 tex; u16 texPltt; u16 clrImg; u16 bgExtPltt; u16 objExtPltt; u16 subBg; } UnkGXState_0200330c;
extern UnkGXState_0200330c data_0207cfe8;
void GX_VRAMCNT_SetLCDC_(u32 lcdc);
#define VRAMCNT_C (*(vu8 *)0x04000242)
#define VRAMCNT_H (*(vu8 *)0x04000248)
#define VRAMCNT_I (*(vu8 *)0x04000249)
void GX_SetBankForSubBG(s32 v)
{
    data_0207cfe8.lcdc = (u16)(~v & (data_0207cfe8.lcdc | data_0207cfe8.subBg));
    data_0207cfe8.subBg = (u16)v;

    if (v <= 0x80) {
        if (v >= 0x80) goto h;
        if (v > 4) goto end;
        if (v < 0) goto end;
        if (v == 0) goto end;
        if (v == 4) VRAMCNT_C = 0x84;
        goto end;
    }
    if (v != 0x180) goto end;
    VRAMCNT_I = 0x81;
h:
    VRAMCNT_H = 0x81;
end:

    GX_VRAMCNT_SetLCDC_(data_0207cfe8.lcdc);
}
