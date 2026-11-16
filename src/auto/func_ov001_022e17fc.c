/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02002ab0();
extern u8 data_0208926c[];
s32 GXx_SetMasterBrightness_();
extern u8 gDisplayMain[];

void func_ov001_022e17fc(void) {
    func_02002ab0(1, 3, 1);
    *(volatile u16 *)0x400000a = ((*(volatile u16 *)0x400000a & 0x43) | 0x2484);
    *(volatile u16 *)0x400000c = ((*(volatile u16 *)0x400000c & 0x43) | 0x588);
    *(volatile u16 *)0x400000a = ((*(volatile u16 *)0x400000a & ~3) | 1);
    *(volatile u16 *)0x400000c = (*(volatile u16 *)0x400000c & ~3);
    *(u16 *)((u8 *)data_0208926c + 0x8) = 0x10;
    GXx_SetMasterBrightness_(0x400006c, *(s16 *)((u8 *)data_0208926c + 8), data_0208926c, 3);
    *(u16 *)((u8 *)gDisplayMain + 0xa) = 0;
    *(u16 *)((u8 *)gDisplayMain + 0x8) = *(u16 *)((u8 *)gDisplayMain + 0xa);
    *(u16 *)((u8 *)gDisplayMain + 0x6) = *(u16 *)((u8 *)gDisplayMain + 0x8);
    *(u16 *)((u8 *)gDisplayMain + 0x4) = *(u16 *)((u8 *)gDisplayMain + 0x6);
    *(volatile u32 *)0x4000014 = 0;
    *(volatile u32 *)0x4000018 = 0;
    *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & -7937) | 0x1600);
}

#pragma thumb reset
