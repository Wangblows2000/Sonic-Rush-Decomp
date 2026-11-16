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
    u16 subBg;
    u16 subObj;
    u16 subBgExtPltt;
    u16 subObjExtPltt;
} UnkGXState_02003290;

extern UnkGXState_02003290 data_0207cfe8;

void GX_VRAMCNT_SetLCDC_(u32 lcdc);

#define VRAMCNT_C (*(vu8 *)0x04000242)
#define VRAMCNT_D (*(vu8 *)0x04000243)
#define VRAMCNT_H (*(vu8 *)0x04000248)
#define VRAMCNT_I (*(vu8 *)0x04000249)
#define DISPCNT_SUB (*(vu32 *)0x04001000)

void func_02003290(s32 v)
{
    data_0207cfe8.lcdc = (u16)(~v & (data_0207cfe8.lcdc | data_0207cfe8.subObj));
    data_0207cfe8.subObj = (u16)v;
    switch (v) {
    case 8:
        VRAMCNT_D = 0x84;
        break;
    case 0x100:
        VRAMCNT_I = 0x82;
        break;
    case 0:
        break;
    }
    GX_VRAMCNT_SetLCDC_(data_0207cfe8.lcdc);
}
