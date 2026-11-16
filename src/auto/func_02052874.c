/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Vram_SetBankForSubOBJ();
s32 ObjVram_Reset();
s32 Vram_SetBankForSubBG();
s32 GXS_SetGraphicsMode();
s32 Vram_SetBankForSubBGExtPltt();
extern u8 gDisplaySub[];

void func_02052874(s32 p0) {
    if ((s32)p0 == 0) {
        Vram_SetBankForSubOBJ(0x100, 0x100010, 0x40, 0, 0x100);
        ObjVram_Reset(1);
        Vram_SetBankForSubBG(4);
    }
    GXS_SetGraphicsMode(0);
    Vram_SetBankForSubBGExtPltt(0);
    *(volatile u16 *)0x4001008 = ((*(volatile u16 *)0x4001008 & 0x43) | 0x2610);
    *(volatile u16 *)0x400100a = ((*(volatile u16 *)0x400100a & 0x43) | 0x2708);
    *(volatile u16 *)0x400100c = ((*(volatile u16 *)0x400100c & 0x43) | 0x810);
    *(volatile u16 *)0x400100e = ((*(volatile u16 *)0x400100e & 0x43) | 0x900);
    *(volatile u16 *)0x4001008 = ((*(volatile u16 *)0x4001008 & ~3) | 1);
    *(volatile u16 *)0x400100a = ((*(volatile u16 *)0x400100a & ~3) | 2);
    *(volatile u16 *)0x400100c = ((*(volatile u16 *)0x400100c & ~3) | 3);
    *(volatile u16 *)0x400100e = (*(volatile u16 *)0x400100e & ~3);
    *(volatile u32 *)0x4001010 = 0;
    *(u16 *)gDisplaySub = 0;
    *(u16 *)((u8 *)gDisplaySub + 0x2) = 0;
    *(volatile u32 *)0x4001014 = 0;
    *(u16 *)((u8 *)gDisplaySub + 0x4) = 0;
    *(u16 *)((u8 *)gDisplaySub + 0x6) = 0;
    *(volatile u32 *)0x4001018 = 0;
    *(u16 *)((u8 *)gDisplaySub + 0x8) = 0;
    *(u16 *)((u8 *)gDisplaySub + 0xa) = 0;
    *(u32 *)((u8 *)gDisplaySub + 0x1c) = 0;
    *(volatile u32 *)0x4001000 = (*(volatile u32 *)0x4001000 & ~0xe000);
}
